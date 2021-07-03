import { UnitLabel } from "renderer/components";
import styles from "./UnitButton.scss";

// prop types
interface UnitButtonProps {
    active?: boolean,
    label: string,
    onPress: () => void,
}

// button on audio unit
const UnitButton = ({ active, label, onPress }: UnitButtonProps) => (
    <button
        className={`${styles.root} ${active ? styles.active : ""}`}
        type="button"
        onClick={onPress}
    >
        <UnitLabel text={label} />
    </button>
);

export default UnitButton;
