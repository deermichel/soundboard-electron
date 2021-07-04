module.exports = {
    env: {
        "browser": true,
        "es6": true,
        "node": true,
    },
    extends: [
        "eslint:recommended",
        "plugin:@typescript-eslint/eslint-recommended",
        "plugin:@typescript-eslint/recommended",
        "plugin:import/errors",
        "plugin:import/warnings",
        "plugin:import/typescript",
        "plugin:react/recommended",
        "plugin:react-hooks/recommended",
    ],
    parser: "@typescript-eslint/parser",
    rules: {
        "@typescript-eslint/comma-dangle": ["error", "always-multiline"],
        "@typescript-eslint/explicit-module-boundary-types": "off",
        "@typescript-eslint/indent": ["error", 4],
        "@typescript-eslint/quotes": ["error", "double"],
        "react/jsx-indent": ["error", 4],
        "react/jsx-indent-props": ["error", 4],
        "react/react-in-jsx-scope": "off",
        "react-hooks/exhaustive-deps": "off",
    },
    settings: {
        "import/resolver": {
            node: {
                extensions: [".js", ".jsx", ".ts", ".tsx"],
            },
            typescript: {},
        },
    },
};
