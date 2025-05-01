# Weekly Report – Sprint 2 Week 7

## Overview  
This report outlines the progress made during Week 7 of Sprint 2. The primary focus this week was the design and implementation of a new **Backpack System** to enable item interaction within the game. This includes item pickup, drop functionality, and a UI component to display collected items. The team also continued tracking and investigating previously identified movement bugs.

---

## Completed Tasks

### Backpack System Implementation
- Developed a basic **Backpack System** to support in-game item interaction
- Players can now:
  - Press **`F`** to **pick up items** in the Dungeon
  - View collected items in a **persistent inventory bar UI** at the bottom of the screen
  - Press **`G`** to **drop items** back into the world
- Ensured that item state updates correctly in both gameplay and UI
- Inventory UI dynamically reflects additions and removals in real time
  <table>
      <tr>
        <th>Picking up system and backpack bar</th>
      </tr>
      <tr>
        <td><img src="https://github.com/southern-cross-ai/Deep_Dark_Dungeon/blob/main/assets/weekly_report_7_assets_1.png" width="400"/></td>
      </tr>
    </table>

---

## Bugs Identified

| Bug Title                             | Description                                                                                                     | Status     |
|--------------------------------------|-----------------------------------------------------------------------------------------------------------------|------------|
| Movement Loss After System Transition | When launching the game from the **Base System**, teleporting to Exploration or Dungeon results in movement loss. Similarly, if the game starts from the **Dungeon System**, transitioning to the other systems also causes the player to be immobile. | Identified |
| Inverted Movement in Dungeon          | Inside the **Dungeon System**, player movement becomes inverted. Clicking a destination moves the player in the opposite direction. Additionally, horizontal movement is disabled; only vertical movement (up/down) works. | Identified |

---

## Observations and Notes
- The newly added Backpack System improves player interaction and paves the way for inventory-dependent features (e.g., quest items, equipment).
- Movement issues from prior sprints remain unresolved and appear related to:
  - Player controller state reinitialization
  - Input system binding loss across scenes
  - Dungeon navigation mesh configuration

---

## Plans Moving Forward
- Expand the Backpack System to support multiple item types, stacking, and limited inventory size
- Fix known movement issues through debugging and controller rebind strategies
- Begin linking backpack contents to game logic (e.g., triggers, quest progress)
- Continue preparing for Game Instance System integration in Sprint 2
