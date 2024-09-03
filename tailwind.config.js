/** @type {import('tailwindcss').Config} */
module.exports = {
	content: [
		"./src/resources/**/*.html",
		"./src/resources/*.html"
	],
	theme: {
		extend: {
                        colors: {
                                background: "#141414",
                                foreground: "#d8d8d8",
                                shade: "#181818",
                                lighter: "#242424",
                                lightaf: "#323232",
                        },
                },
	},
	plugins: [],
}

