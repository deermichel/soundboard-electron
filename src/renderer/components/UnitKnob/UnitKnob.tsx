import { UnitLabel } from "renderer/components";
import { useEffect, useRef, useState } from "react";
import styles from "./UnitKnob.scss";

// prop types
interface UnitKnobProps {
    label: string,
    value?: number,
    onChange?: (value: number) => void,
}

// knob on audio unit
const UnitKnob = ({ label, value, onChange }: UnitKnobProps) => {
    // state
    const [isMouseDown, setMouseDown] = useState(false);
    const [localValue, setLocalValue] = useState(0);

    // value ref for mouseMove listener
    const valueRef = useRef(localValue);
    useEffect(() => { valueRef.current = localValue; }, [localValue]);

    // sync local value
    useEffect(() => {
        if (value && !isMouseDown) {
            setLocalValue(value);
        }
    }, [isMouseDown, value]);

    // gui calc helpers (mimic behavior of fader)
    const valueToKnob = (val: number) => (1 - val) * 200;
    const knobToValue = (knob: number) => 1 - (knob / 200);

    // mouse event listeners
    const mouseDown = () => setMouseDown(true);
    const mouseUp = () => setMouseDown(false);
    const mouseMove = (e: PointerEvent) => {
        const newValue = knobToValue(valueToKnob(valueRef.current) + e.movementY * (e.shiftKey ? 0.1 : 1.0));
        const clippedValue = Math.max(0, Math.min(1, newValue));
        setLocalValue(clippedValue);
        if (onChange) onChange(clippedValue);
    };

    // un/register mouse event listeners
    useEffect(() => {
        if (isMouseDown) {
            window.addEventListener("pointerup", mouseUp);
            window.addEventListener("pointermove", mouseMove);
            return () => {
                window.removeEventListener("pointerup", mouseUp);
                window.removeEventListener("pointermove", mouseMove);
            };
        }
        return () => {};
    }, [isMouseDown]);

    // render
    return (
        <div className={styles.root}>
            <div className={`${styles.knob} ${isMouseDown ? styles.mouseDown : ""}`} onPointerDown={mouseDown}>
                <div className={styles.mark} style={{ transform: `rotateZ(${135 + localValue * 270}deg) translateX(32px)` }} />
            </div>
            <UnitLabel text={label} />
        </div>
    );
};

export default UnitKnob;
