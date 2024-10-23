const withMultiPlatform = (platformValues) => {
    if (navigator.userAgent.indexOf("Mac OS X") !== -1) {
        return platformValues.macos;
    } else {
        return platformValues.linux;
    }
}

const homePath = (username) => withMultiPlatform({
    linux: `/home/${username}/`,
    macos: `/Users/${username}/`,
});