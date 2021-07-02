import { AudioUnitPlaceholder } from "renderer/components";
import { AudioUnitId, AudioUnitType } from "backend/types";
import styles from "./ChannelStrip.scss";

// prop types
interface ChannelStripProps {
    channelStripIndex: number,
}

// container for multiple audio units
const ChannelStrip = ({ channelStripIndex }: ChannelStripProps) => {
    // on add audio unit
    const onAddAudioUnit = (id: AudioUnitId | null, index: number) => {
        console.log(id, index);
    };

    // render
    return (
        <div className={styles.root}>
            <AudioUnitPlaceholder type={AudioUnitType.Instrument} onSelect={(id) => onAddAudioUnit(id, 0)} />
        </div>
    );
};

export default ChannelStrip;
