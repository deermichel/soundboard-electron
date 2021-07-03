import { UnitButton, UnitKnob, UnitFader } from "renderer/components";
import { AudioUnitControl, AudioUnitControlType } from "backend/types";
import styles from "./ControlContainer.scss";

// prop types
interface ControlContainerProps {
    control: AudioUnitControl
    value: number,
    onChange: (value: number) => void,
}

// audio unit control container
const ControlContainer = ({ control, value, onChange }: ControlContainerProps) => {
    const { label, type, x, y } = control;

    // render
    const margin = 16;
    const controlHeight = 124;
    const controlWidth = 96;
    const transformX = margin + x * (controlWidth + margin);
    const transformY = margin + y * (controlHeight + margin);
    return (
        <div className={styles.root} style={{ transform: `translate(${transformX}px, ${transformY}px)` }}>
            {/* button */}
            {type === AudioUnitControlType.Button && (
                <div className={styles.button}>
                    <UnitButton label={label} active={!!value} onPress={() => onChange(value ? 0 : 1)} />
                </div>
            )}

            {/* fader */}
            {type === AudioUnitControlType.Fader && (
                <UnitFader label={label} value={value} onChange={onChange} />
            )}

            {/* knob */}
            {type === AudioUnitControlType.Knob && (
                <UnitKnob label={label} value={value} onChange={onChange} />
            )}
        </div>
    );
};

export default ControlContainer;
