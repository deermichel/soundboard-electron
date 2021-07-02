import CloseIcon from "assets/icons/close.svg";
import SettingsIcon from "assets/icons/settings.svg";
import { setEditMode } from "renderer/store/app";
import { useAppDispatch, useAppSelector } from "renderer/store/hooks";
import styles from "./MenuBar.scss";

// menu bar at top of screen
const MenuBar = () => {
    // redux
    const dispatch = useAppDispatch();
    const editMode = useAppSelector((state) => state.editMode);

    // render
    return (
        <div className={styles.root}>
            <button type="button" className={styles.button} onClick={() => dispatch(setEditMode(!editMode))}>
                {editMode ? (
                    <CloseIcon style={{ width: 40, height: 40 }} />
                ) : (
                    <SettingsIcon style={{ width: 32, height: 32 }} />
                )}
            </button>
        </div>
    );
};

export default MenuBar;
