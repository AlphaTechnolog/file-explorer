/** @type {import('tailwindcss').Config} */
module.exports = {
    content: [
        "./src/resources/**/*.{html,js}",
        "./src/resources/*.{html,js}"
    ],
    theme: {
        extend: {
            colors: {
                background: "#141414",
                foreground: "#d8d8d8",
                shade: "#181818",
                lighter: "#242424",
                lightaf: "#323232",
                primary: "#008c77",
                yellow: "#e0af68"
            },
        },
    },
    plugins: [],
}

