import AddIcon from "assets/icons/plus.svg";
import EditIcon from "assets/icons/edit.svg";
import styles from "./MenuButtons.scss";

// prop types
interface MenuButtonsProps {
    onAdd?: () => void,
    onEdit?: () => void,
}

// channel strip menu buttons on the right side
const MenuButtons = ({ onAdd, onEdit }: MenuButtonsProps) => (
    <div className={styles.root} style={{ transform: `translate(56px, ${onAdd ? "-48px" : "0px"})` }}>
        {onAdd && (
            <button className={styles.button} type="button" onClick={onAdd}>
                <AddIcon style={{ width: 32, height: 32 }} />
            </button>
        )}
        {onEdit && (
            <button className={styles.button} type="button" onClick={onEdit}>
                <EditIcon style={{ width: 24, height: 24 }} />
            </button>
        )}
    </div>
);

export default MenuButtons;
