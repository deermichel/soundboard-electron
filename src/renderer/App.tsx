import { useEffect } from "react";
import { ChannelStrip, MenuBar } from "renderer/components";
import { useAppDispatch, useAppSelector } from "renderer/store/hooks";
import styles from "./App.scss";
import { updateParameterValues, View } from "./store/app";

// main app container
const App = () => {
    // redux
    const dispatch = useAppDispatch();
    const channelStrips = useAppSelector((state) => state.session.channelStrips);
    const view = useAppSelector((state) => state.view);

    // frequently poll parameter values
    useEffect(() => {
        const id = setInterval(() => {
            dispatch(updateParameterValues());
        }, 500);
        return () => clearTimeout(id);
    }, []);

    // render
    const editMode = view === View.Edit;
    return (
        <div className={styles.root}>
            <MenuBar />
            {(view === View.Edit || view === View.Perform) && (
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
            )}
        </div>
    );
};

export default App;
