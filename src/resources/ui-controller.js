/** @type {HTMLInputElement} */
const $locationInput = document.querySelector("#location-input");

/** @type {HTMLParagraphElement} */
const $totalCount = document.querySelector("#total-count");

/** @type {HTMLDivElement} */
const $noContent = document.querySelector("#no-contents");

/** @type {HTMLDivElement} */
const $contentGrid = document.querySelector("#content-grid");

/** @type {HTMLButtonElement} */
const $goBackButton = document.querySelector("#go-back-button");

/** @type {HTMLButtonElement} */
const $goUpButton = document.querySelector("#go-up-button");

let locationValue = "";
let cachedUsername = undefined;

const updateUsername = async () => {
    const { username } = await request("obtainUsername");
    cachedUsername = username;
}

/**
 * Converts ~ into $HOME
 * @param {string} value
 * @returns Expanded value
 */
const expandTilde = async (value) => {
    if (cachedUsername === undefined) await updateUsername();
    return value.replace("~", homePath(cachedUsername));
}

/**
 * Replaces /home/username with ~
 * @param {string} value
 * @returns The converted value
 */
const compressUsername = async (value) => {
    if (cachedUsername === undefined) await updateUsername();
    return value.replace(homePath(cachedUsername), "~");
}

const showNoContent = (value) => {
    $noContent.style.display = Boolean(value) ? "flex" : "none";
}

const clearContentGrid = () => {
    while ($contentGrid.hasChildNodes()) {
        $contentGrid.firstChild.remove();
    }
}

const showContentGrid = (value) => {
    $contentGrid.style.display = Boolean(value) ? "grid" : "none";
}

/**
 * Limits a string by n
 * @param {string} value
 * @param {number} n
 * @returns {string}
 */
const limit = (value, n) => {
    const suffix = "...";
    if (value.length >= n) {
        return value.slice(0, n) + suffix;
    }

    return value;
}

/**
 * Returns the html of an entry.
 * @param {"folder" | "file"} kind
 * @param {string} name
 */
function ExplorerEntry(kind, name) {
    const maxChars = 14;

    if (kind === "file") {
        return /*html*/`
            <button class="flex justify-center items-center w-full flex-col text-lightaf transition-colors duration-300 hover:bg-shade rounded-lg text-sm py-4 pb-6" data-name="${name}">
                <svg xmlns="http://www.w3.org/2000/svg" height="64px" viewBox="0 -960 960 960" width="64px" fill="currentColor"><path d="M320-240h320v-80H320v80Zm0-160h320v-80H320v80ZM240-80q-33 0-56.5-23.5T160-160v-640q0-33 23.5-56.5T240-880h320l240 240v480q0 33-23.5 56.5T720-80H240Zm280-520v-200H240v640h480v-440H520ZM240-800v200-200 640-640Z"/></svg>
                <p class="text-center text-foreground">
                    ${limit(name, maxChars)}
                </p>
            </button>
        `
    }

    return /*html*/`
        <button class="flex justify-center items-center w-full flex-col text-primary transition-colors duration-300 hover:bg-shade rounded-lg text-sm py-maxChars pb-6" data-name="${name}" id="explorer-directory-entry">
            <svg xmlns="http://www.w3.org/2000/svg" height="64px" viewBox="0 -960 960 960" width="64px" fill="currentColor"><path d="M160-160q-33 0-56.5-23.5T80-240v-480q0-33 23.5-56.5T160-800h240l80 80h320q33 0 56.5 23.5T880-640v400q0 33-23.5 56.5T800-160H160Zm0-80h640v-400H447l-80-80H160v480Zm0 0v-480 480Z"/></svg>
            <p class="text-center text-foreground">
                ${limit(name, maxChars)}
            </p>
        </button>
    `
}

const addDirectoryListeners = () => {
    /** @type {HTMLButtonElement[]} */
    const buttons = document.querySelectorAll("#explorer-directory-entry");

    for (const button of buttons) {
        button.addEventListener("click", async () => {
            const { name } = button.dataset;
            clearContentGrid();
            navigateTo(name);
        });
    }
}

const updateContents = async () => {
    if (context.total === 0) {
        showContentGrid(false);
        showNoContent(true);
        return;
    }

    showContentGrid(true);
    showNoContent(false);

    context.directories.forEach((dirname) => {
        $contentGrid.innerHTML += ExplorerEntry("folder", dirname);
    });

    context.files.forEach((filename) => {
        $contentGrid.innerHTML += ExplorerEntry("file", filename);
    });

    addDirectoryListeners();
}

const refreshUI = async () => {
    $locationInput.value = await compressUsername(context.currentDirectory);
    $totalCount.textContent = `${context.total} Elements`;
    locationValue = $locationInput.value;
    
    updateContents();
}

/**
 * Handles the location input submit event, also executed when on blur
 * @param {Event} e
 */
const handleLocationInputSubmit = async e => {
    $locationInput.value = await compressUsername(e.target.value);
    const value = await expandTilde(e.target.value);
    const contents = await request("checkDirname", value);
    if (!contents.valid) {
        e.target.value = locationValue;
        return;
    }

    // if it's correct, let's save it again.
    locationValue = e.target.value;

    clearContentGrid();
    navigateAbsolute((await expandTilde(locationValue)));
}

$locationInput.addEventListener("blur", handleLocationInputSubmit);

$locationInput.addEventListener("keypress", e => {
    if (e.key == "Enter") {
        handleLocationInputSubmit(e);
    }
});

$goUpButton.addEventListener("click", () => {
    const dir = context.currentDirectory.split("/");
    clearContentGrid();
    navigateAbsolute(dir.slice(0, dir.length - 2).join("/"));
});

$goBackButton.addEventListener("click", () => {
    if (dirHistory.length <= 1) return;
    clearContentGrid();
    goDirBack();
});