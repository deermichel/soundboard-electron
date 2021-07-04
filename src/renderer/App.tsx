import { useEffect } from "react";
import { ChannelStrip, MenuBar } from "renderer/components";
import { useAppDispatch, useAppSelector } from "renderer/store/hooks";
import styles from "./App.scss";
import { updateParameterValues } from "./store/app";

// main app container
const App = () => {
    // redux
    const dispatch = useAppDispatch();
    const channelStrips = useAppSelector((state) => state.session.channelStrips);
    const editMode = useAppSelector((state) => state.editMode);

    // frequently poll parameter values
    useEffect(() => {
        const id = setInterval(() => {
            dispatch(updateParameterValues());
        }, 500);
        return () => clearTimeout(id);
    }, []);

    // render
    return (
        <div className={styles.root}>
            <MenuBar />
            <div className={styles.channelStripsContainer} style={{ flex: editMode ? 1 : 0 }}>
                {channelStrips.map((_, i) => (
                    <div key={`channel${i}`} className={styles.channelStrip}>
                        <ChannelStrip channelStripIndex={i} />
                    </div>
                ))}
                {editMode && (
                    <div className={styles.channelStrip}>
                        <ChannelStrip channelStripIndex={channelStrips.length} />
                    </div>
                )}
            </div>
        </div>
    );
};

export default App;
