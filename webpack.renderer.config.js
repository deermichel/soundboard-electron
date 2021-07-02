const rules = require("./webpack.rules");
const plugins = require("./webpack.plugins");
const TsConfigPathsPlugin = require('tsconfig-paths-webpack-plugin');

rules.push({
    test: /\.scss$/,
    use: [
        { loader: "style-loader" },
        { loader: "css-loader", options: { modules: true } },
        { loader: "sass-loader" },
    ],
});
rules.push({
    test: /\.svg$/,
    use: [{ loader: "@svgr/webpack", options: { dimensions: false } }],
});

module.exports = {
    module: {
        rules,
    },
    plugins: plugins,
    resolve: {
        extensions: [".js", ".ts", ".jsx", ".tsx", ".scss"],
        plugins: [new TsConfigPathsPlugin()],
    },
};
