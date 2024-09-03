const context = {};
const loading = false;

const sleep = ms => new Promise((resolve) => {
    setTimeout(resolve, ms);
});

const reloadContext = async () => {
    const { username } = await window.obtainUsername();
    context.currentDirectory = `/home/${username}`;
}

const main = async () => {
    await reloadContext();
}

main();