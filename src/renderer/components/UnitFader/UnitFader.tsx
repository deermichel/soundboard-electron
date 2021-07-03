import { UnitLabel } from "renderer/components";
import { useEffect, useRef, useState } from "react";
import styles from "./UnitFader.scss";

// prop types
interface UnitFaderProps {
    label: string,
    value: number,
    onChange: (value: number) => void,
}

// fader on audio unit
const UnitFader = ({ label, value, onChange }: UnitFaderProps) => {
    // state
    const [isMouseDown, setMouseDown] = useState(false);

    // value ref for mouseMove listener
    const valueRef = useRef(value);
    useEffect(() => { valueRef.current = value; }, [value]);

    // gui calc helpers
    const travelDistance = 236 - 80;
    const valueToKnob = (val: number) => (1 - val) * travelDistance;
    const knobToValue = (knob: number) => 1 - (knob / travelDistance);

    // mouse event listeners
    const mouseDown = () => setMouseDown(true);
    const mouseUp = () => setMouseDown(false);
    const mouseMove = (e: PointerEvent) => { // TODO: fix Safari & iOS mouseMove not working
        const newValue = knobToValue(valueToKnob(valueRef.current) + e.movementY * (e.shiftKey ? 0.1 : 1.0));
        onChange(Math.max(0, Math.min(1, newValue)));
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
            <div className={`${styles.fader} ${isMouseDown ? styles.mouseDown : ""}`} onPointerDown={mouseDown}>
                <div className={styles.track} />
                <div className={styles.knob} style={{ transform: `translateY(${valueToKnob(value)}px)` }}>
                    <div className={styles.mark} />
                </div>
            </div>
            <UnitLabel text={label} />
        </div>
    );
};

export default UnitFader;
