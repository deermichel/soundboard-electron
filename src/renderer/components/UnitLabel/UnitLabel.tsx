import styles from "./UnitLabel.scss";

// prop types
interface UnitLabelProps {
    text: string,
}

// label on unit element
const UnitLabel = ({ text }: UnitLabelProps) => (
    <div className={styles.root}>
        {text}
    </div>
);

export default UnitLabel;
