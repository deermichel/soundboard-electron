import { ChannelStrip, MenuBar } from "renderer/components";
import { useAppSelector } from "renderer/store/hooks";
import styles from "./App.scss";

// main app container
const App = () => {
    // redux
    const channelStrips: string[] = [];
    const editMode = useAppSelector((state) => state.editMode);

    // render
    return (
        <div className={styles.root}>
            <MenuBar />
            <div className={styles.channelStripsContainer} style={{ flex: editMode ? 1 : 0 }}>
                {channelStrips.map((_, i) => (
                    <div key={i} className={styles.channelStrip}>
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
