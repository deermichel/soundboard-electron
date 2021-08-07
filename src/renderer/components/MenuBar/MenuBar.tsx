import CheckIcon from "assets/icons/check.svg";
import CloseIcon from "assets/icons/close.svg";
import SettingsIcon from "assets/icons/settings.svg";
import SpeakerIcon from "assets/icons/speaker.svg";
import { setView, View } from "renderer/store/app";
import { useAppDispatch, useAppSelector } from "renderer/store/hooks";
import styles from "./MenuBar.scss";

// menu bar at top of screen
const MenuBar = () => {
    // redux
    const dispatch = useAppDispatch();
    const view = useAppSelector((state) => state.view);

    // render
    return (
        <div className={styles.root}>
            {view === View.Devices && (
                <>
                    <button type="button" className={styles.iconButton} onClick={() => dispatch(setView(View.Edit))}>
                        <CloseIcon style={{ width: 40, height: 40 }} />
                    </button>
                    <button type="button" className={styles.iconButton} onClick={() => dispatch(setView(View.Edit))}>
                        <CheckIcon style={{ width: 40, height: 40 }} />
                    </button>
                </>
            )}
            {view === View.Edit && (
                <>
                    <button type="button" className={styles.button} onClick={() => dispatch(setView(View.Devices))}>
                        <SpeakerIcon style={{ width: 32, height: 32 }} />
                        <span>Devices</span>
                    </button>
                    <button type="button" className={styles.iconButton} onClick={() => dispatch(setView(View.Perform))}>
                        <CloseIcon style={{ width: 40, height: 40 }} />
                    </button>
                </>
            )}
            {view === View.Perform && (
                <button type="button" className={styles.iconButton} onClick={() => dispatch(setView(View.Edit))}>
                    <SettingsIcon style={{ width: 32, height: 32 }} />
                </button>
            )}
        </div>
    );
};

export default MenuBar;
