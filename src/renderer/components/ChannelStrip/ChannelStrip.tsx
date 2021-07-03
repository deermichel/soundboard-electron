import { useEffect, useState } from "react";
import { AudioUnitPlaceholder, AudioUnit } from "renderer/components";
import { AudioUnitId, AudioUnitType } from "backend/types";
import { useAppDispatch, useAppSelector } from "renderer/store/hooks";
import { addChannelStrip, insertAudioUnit } from "renderer/store/app";
import styles from "./ChannelStrip.scss";
import MenuButtons from "./MenuButtons";

// prop types
interface ChannelStripProps {
    channelStripIndex: number,
}

// container for multiple audio units
const ChannelStrip = ({ channelStripIndex }: ChannelStripProps) => {
    // redux
    const dispatch = useAppDispatch();
    const audioUnits = useAppSelector((state) => state.session?.channelStrips[channelStripIndex]?.audioUnits || []);
    const editMode = useAppSelector((state) => state.editMode);
    const [showPlaceholder, setShowPlaceholder] = useState<{ atIndex: number, mode: "add" | "edit" }>(); // show audio unit placeholder at index
    const isPlaceholder = audioUnits.length === 0;

    // hide placeholder when edit mode is cancelled
    useEffect(() => { if (!editMode) setShowPlaceholder(undefined); }, [editMode]);

    // on add audio unit
    const onAddAudioUnit = (id: AudioUnitId | null, index: number) => {
        setShowPlaceholder(undefined); // hide placeholder
        if (!id) return; // none
        if (isPlaceholder) {
            dispatch(addChannelStrip()); // add channel strip
        }
        dispatch(insertAudioUnit(id, channelStripIndex, index)); // add audio unit
    };

    // on replace audio unit
    const onReplaceAudioUnit = (id: AudioUnitId | null, index: number) => {
        setShowPlaceholder(undefined); // hide placeholder
        if (id === audioUnits[index].id) return; // same as before
        if (id) {
            // TODO: new api call -> replace
            // dispatch(api.session.removeAudioUnit(channelStripIndex, index));
            // dispatch(api.session.insertAudioUnit(id, channelStripIndex, index));
        } else if (index === 0) {
            // TODO: confirm dialog if channel strip contains units after instrument
            // dispatch(api.session.removeChannelStrip(index)); // none on first audio unit = remove channel strip
        } else {
            // dispatch(api.session.removeAudioUnit(channelStripIndex, index)); // none = remove audio unit
        }
    };

    // TODO
    const showAddButton = (index: number) => index !== 0 && !(showPlaceholder?.mode === "add" && showPlaceholder.atIndex === index) && !(showPlaceholder?.mode === "edit" && showPlaceholder?.atIndex === index - 1);
    const showEditButton = (index: number) => index !== audioUnits.length - 1 && !(showPlaceholder?.mode === "edit" && showPlaceholder?.atIndex === index);

    // render
    return (
        <div className={styles.root}>
            {isPlaceholder ? (
                <AudioUnitPlaceholder type={AudioUnitType.Instrument} onSelect={(id) => onAddAudioUnit(id, 0)} />
            ) : (
                audioUnits.map((unit, index) => (
                    // eslint-disable-next-line
                    <div key={index}>
                        {/* placeholder if shown at this index */}
                        <div style={{ transition: ".1s ease-out", margin: showPlaceholder?.mode === "add" && showPlaceholder?.atIndex === index ? "8px 0" : "" }}>
                            {showPlaceholder?.mode === "add" && showPlaceholder.atIndex === index && (
                                <AudioUnitPlaceholder // onAppear margin animation?
                                    allowNone
                                    type={AudioUnitType.Effect}
                                    onSelect={(id) => onAddAudioUnit(id, index)}
                                />
                            )}
                        </div>

                        {/* actual audio unit with menu buttons */}
                        <div className={styles.unitContainer}>
                            <div
                                style={{
                                    transition: ".1s ease-out",
                                    marginBottom: showPlaceholder?.mode === "edit" && showPlaceholder?.atIndex === index ? 8 : 0,
                                    marginTop: showPlaceholder?.mode === "edit" && showPlaceholder?.atIndex === index && index > 0 ? 8 : 0,
                                }}
                            >
                                {showPlaceholder?.mode === "edit" && showPlaceholder.atIndex === index ? (
                                    <AudioUnitPlaceholder
                                        allowNone
                                        edit={unit.id}
                                        type={index === 0 ? AudioUnitType.Instrument : AudioUnitType.Effect}
                                        onSelect={(id) => onReplaceAudioUnit(id, index)}
                                    />
                                ) : (
                                    <AudioUnit
                                        descriptionId={unit.id}
                                        values={{}}
                                        onChange={(controlId, value) => console.log(index, controlId, value)}
                                    />
                                )}
                            </div>
                            {editMode && (
                                <MenuButtons
                                    onAdd={showAddButton(index) ? () => setShowPlaceholder({ atIndex: index, mode: "add" }) : undefined}
                                    onEdit={showEditButton(index) ? () => setShowPlaceholder({ atIndex: index, mode: "edit" }) : undefined}
                                />
                            )}
                        </div>
                    </div>
                ))
            )}
        </div>
    );
};

export default ChannelStrip;
