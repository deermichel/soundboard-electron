module.exports = {
    "makers": [],
    "plugins": [
        [
            "@electron-forge/plugin-webpack",
            {
                "mainConfig": "./webpack.main.config.js",
                "renderer": {
                    "config": "./webpack.renderer.config.js",
                    "entryPoints": [
                        {
                            "html": "./src/renderer/index.html",
                            "js": "./src/renderer/index.tsx",
                            "name": "main_window",
                            "preload": {
                                "js": "./src/renderer/preload.ts",
                            },
                        },
                    ],
                },
            },
        ],
    ],
};
