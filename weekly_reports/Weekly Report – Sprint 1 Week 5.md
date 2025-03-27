# Weekly Report – Sprint 1 Week 5

## Overview  
This report outlines the progress made during Week 5 of Sprint 1. The team focused on completing code reviews, integrating subsystems into the main project, cleaning up project assets, and initiating work on the game instance system. Key bugs were identified during early integration testing.

---

## Completed Tasks

### System Code Reviews
- Completed code review of the **Exploration System**
  - Merged exploration components into the `main` branch
  - Removed unused assets and deprecated scripts
  - Determined that full Blueprint conversion is not feasible due to strong C++ dependencies

- Finalized review and integration of the **Dungeon System**
  - Cleaned up the project structure
  - Removed redundant materials and legacy components
  - Ensured compatibility with upcoming instance management
      **Before vs After Comparison**

    <table>
    <tr>
      <th>Before</th>
      <th>After</th>
    </tr>
    <tr>
      <td><img src="assets\weekly_report_assets_1.png" width="400"/></td>
      <td><img src="assets\weekly_report_assets_2.png" width="400"/></td>
    </tr>
  </table>

- Completed code review of the **Base System**
  - Due to the system’s complexity and multi-scene navigation logic, significant time was required to fully understand the architecture
  - The original implementation was a mixture of C++ and Blueprints, increasing the difficulty of analysis and maintenance
  - Successfully converted the system into a **Blueprint-only** project to ensure consistency across the codebase
  - Resolved a critical issue where the system was unable to read instance data after the conversion

### Project Cleanup
- Deleted outdated and unused assets from the Dungeon and Explore systems
- Reorganized project folders to improve clarity and maintainability

### Game Instance System (Planning Phase)
- Outlined initial data structure and design considerations for instance-based runtime handling
- After further discussion, we determined that implementing isolated instance systems for each subsystem is not practical
- We concluded that the **game instance system should be deferred** until the core systems (Dungeon, Base, Explore) are fully merged
- This is necessary to define a unified model for **game data storage**, including player progress, inventory, and equipment across all systems

---

## Bugs Identified

| Bug Title                      | Description                                                                 | Status     |
|-------------------------------|-----------------------------------------------------------------------------|------------|
| Player Movement During Dialogue | Player is still able to move during active dialogue, causing interaction issues | Identified |
| E Key Spam Behavior            | Repeated E key presses result in input lock or unintended repeated actions  | Identified |
| Dialogue and Teleport Conflict | Player entering a teleport zone during dialogue breaks conversation state   | Identified |

---

## Observations and Notes
- The tight integration between Blueprints and C++ in the existing systems limits our ability to convert fully to Blueprints in some areas.
- The current refactoring and cleanup efforts have improved project organization, which will streamline integration and future development.

