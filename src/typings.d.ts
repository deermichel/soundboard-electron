declare module "*.scss" {
    const styles: { [className: string]: string };
    export default styles;
}

declare module "*.svg" {
    const ReactComponent: React.FC<React.SVGProps<SVGSVGElement>>;
    export default ReactComponent;
}
