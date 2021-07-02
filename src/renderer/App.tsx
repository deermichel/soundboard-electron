import styles from "./App.scss";
import { MenuBar } from "renderer/components";

// main app container
const App = () => (
    <div className={styles.root}>
        <MenuBar />
    </div>
);

export default App;
