const sleep = ms => new Promise((resolve) => {
    setTimeout(resolve, ms);
});

const requestsMap = {
    display: window.display,
    obtainUsername: window.obtainUsername,
    obtainDirectories: window.obtainDirectories,
    checkDirname: window.checkDirname
};

const context = {
    currentDirectory: "",
    files: [],
    directories: [],
    total: 0,
};

const dirHistory = [];

const cprint = (...content) => request("display", ...content.map(x => {
    return JSON.stringify(x, null, 2);
}));

const request = (fnName, ...params) => new Promise(async (resolve, reject) => {
    const fn = requestsMap[fnName] ?? (() => new Promise((_, reject) => {
        return reject("Invalid action called from client");
    }));

    const value = await fn(...params).catch(err => {
        cprint(err).then(() => {
            reject();
        });
    });

    resolve(value);
})

const initialCurrentDirectory = async () => {
    const { username } = await request("obtainUsername");
    context.currentDirectory = `/home/${username}/`;
}

/**
 * Navigate into a relative directory to the current work directory.
 * @param {string} directory Relative directory name
 */
const navigateTo = async (directory) => {
    const currentDirectory = (" " + directory).slice(1);

    if (!context.currentDirectory.startsWith("/")) {
        context.currentDirectory += "/";
    }

    context.currentDirectory += directory;

    if (!context.currentDirectory.endsWith("/")) {
        context.currentDirectory += "/";
    }

    const { valid } = await request("checkDirname", context.currentDirectory).catch(() => {
        context.currentDirectory = currentDirectory;
        return;
    });

    if (!valid) {
        await cprint("Unable to set directory to", context.currentDirectory);
        context.currentDirectory = currentDirectory;
        return;
    }

    reloadFiles();
}

const navigateAbsolute = async (absolute) => {
    const currentDirectory = (" " + context.currentDirectory).slice(1);

    context.currentDirectory = absolute;

    if (!context.currentDirectory.endsWith("/")) {
        context.currentDirectory += "/";
    }

    const { valid } = await request("checkDirname", context.currentDirectory).catch(() => {
        context.currentDirectory = currentDirectory;
        return;
    });

    if (!valid) {
        await cprint("navigateAbsolute() Invalid absolute directory " + absolute);
        context.currentDirectory = currentDirectory;
        return;
    }

    reloadFiles();
}

const goDirBack = () => {
    if (dirHistory.length <= 1) return;
    dirHistory.pop();
    context.currentDirectory = dirHistory[dirHistory.length - 1];
    reloadFiles(false);
}

const reloadFiles = async (updateHistory = true) => {
    if (updateHistory)
        dirHistory.push(context.currentDirectory);

    const scanResult = await request("obtainDirectories", context.currentDirectory);
    context.directories = scanResult.directories;
    context.files = scanResult.files;
    context.total = scanResult.total;
    refreshUI();
}

const main = async () => {
    await initialCurrentDirectory();
    await reloadFiles();
}

main();