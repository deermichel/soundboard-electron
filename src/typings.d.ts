// scss files
declare module "*.scss" {
    const styles: { [className: string]: string };
    export default styles;
}

// svg files
declare module "*.svg" {
    const ReactComponent: React.FC<React.SVGProps<SVGSVGElement>>;
    export default ReactComponent;
}

// node modules
declare module "*.node";
