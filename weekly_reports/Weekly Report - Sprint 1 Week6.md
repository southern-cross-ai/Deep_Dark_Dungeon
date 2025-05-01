# Weekly Report – Sprint 1 Week 6

## Overview  
This report outlines the progress made during Week 6 of Sprint 1. The team successfully merged all three core systems (Base, Dungeon, and Exploration) into a unified project structure and implemented teleportation points across the maps to enable system-to-system transitions. While integration was a major milestone, two critical movement-related bugs were identified during testing and are now under investigation.

---

## Completed Tasks

### System Integration
- Fully integrated the **Base**, **Dungeon**, and **Exploration** systems into a single project
- Ensured data structure consistency across all modules
- Resolved key compatibility issues between the systems to support seamless transitions

### Teleportation System Setup
- Implemented teleportation points in all major areas:
  - Exploration → Base / Dungeon
  - Base → Exploration / Dungeon
  - Dungeon → Base / Exploration
- Configured each teleportation point to preserve player state and maintain map-level continuity
- Verified initial scene loading behavior and transitions between systems

### Unified Game Entry Flow
- Set the **Exploration System** as the primary game entry point
- Confirmed that players can start the game in Exploration and teleport to the other two systems successfully

<table>
      <tr>
        <th>Teleporation point</th>
        <th>Teleporation point in dungeon</th>
      </tr>
      <tr>
        <td><img src="https://github.com/southern-cross-ai/Deep_Dark_Dungeon/blob/main/assets/weekly_report_6_assets_1.png" width="400"/></td>
        <td><img src="https://github.com/southern-cross-ai/Deep_Dark_Dungeon/blob/main/assets/weekly_report_6_assets_2.png" width="400"/></td>
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
- Movement issues appear to be linked to **player controller state initialization** and **input handling during system transitions**.
- Navigation mesh or pathfinding logic within dungeon maps may be misconfigured or conflicting with the new integrated structure.
- Teleportation logic is working structurally, but needs to ensure full controller reactivation post-transition.

---

## Plans Moving Forward
- Begin debugging player controller and movement component reinitialization after map loads
- Investigate dungeon map navigation settings and AI pathfinding bindings
- Continue preparing for implementation of the **Game Instance System** in Sprint 2 based on the finalized unified structure
