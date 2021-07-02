import { availableAudioUnits } from "backend";
import { AudioUnitId, AudioUnitType } from "backend/types";
import styles from "./AudioUnitPlaceholder.scss";

// prop types
interface AudioUnitPlaceholderProps {
    allowNone?: boolean,
    edit?: AudioUnitId,
    type: AudioUnitType,
    onSelect: (id: AudioUnitId | null) => void,
}

// placeholder in a channel strip to select instrument or effect audio unit
const AudioUnitPlaceholder = ({ allowNone, edit, type, onSelect }: AudioUnitPlaceholderProps) => {
    // redux
    const audioUnits = Object.values(availableAudioUnits)
        .filter((a) => a.type === type)
        .sort((a1, a2) => a1.name.localeCompare(a2.name));

    // render
    return (
        <div className={styles.root}>
            <div className={styles.header}>{type === AudioUnitType.Instrument ? `${edit ? "Replace" : "New"} Sound` : `${edit ? "Replace" : "Insert"} Effect`}</div>
            <div className={styles.content}>
                {allowNone && (
                    <button type="button" className={`${styles.button} ${styles.internal}`} onClick={() => onSelect(null)}>
                        None
                    </button>
                )}
                {audioUnits.map((audioUnit) => (
                    <button key={audioUnit.id} type="button" className={`${styles.button} ${styles[type]} ${edit === audioUnit.id ? styles.current : ""}`} onClick={() => onSelect(audioUnit.id)}>
                        {audioUnit.name}
                    </button>
                ))}
            </div>
        </div>
    );
};

export default AudioUnitPlaceholder;
