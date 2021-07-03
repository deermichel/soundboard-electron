import { AudioUnitParamId, AudioUnitId, ParameterValue } from "backend/types";
import styles from "./AudioUnit.scss";
import ControlContainer from "./ControlContainer";
import { availableAudioUnits } from "backend";

// prop types
interface AudioUnitProps {
    descriptionId: AudioUnitId,
    values: { [id in AudioUnitParamId]: ParameterValue },
    onChange: (paramId: AudioUnitParamId, value: number) => void,
}

// audio unit in a channel strip
const AudioUnit = ({ descriptionId, values, onChange }: AudioUnitProps) => {
    // redux
    const { controls, name, type } = availableAudioUnits[descriptionId];

    // controls layouting
    const maxY = Math.max(...controls.map((control) => control.y));

    // render
    const margin = 16;
    const controlHeight = 124;
    const height = (maxY + 1) * controlHeight + (maxY + 2) * margin;
    return (
        <div className={styles.root}>
            <div className={`${styles.header} ${styles[type]}`}>{name}</div>
            <div className={styles.content} style={{ height }}>
                {controls.map((control) => (
                    <ControlContainer // eslint-disable-line
                        control={control}
                        // key={control.id} // TODO: fix!!!!
                        value={values[control.id]?.value || 0}
                        onChange={(value) => onChange(control.id, value)}
                    />
                ))}
            </div>
        </div>
    );
};

export default AudioUnit;
