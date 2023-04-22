Reports DFHack plugin
=====================

A DFHack plugin adding a RPC API for retrieving reports (announcements or all reports).

Functions:
----------

 * Reports::GetAnnouncements: dfproto::EmptyMessage → dfproto::Reports::ReportList
   get all announcement reports from `world.status.announcements`
 * Reports::GetReports: df::proto::EmptyMessage → dfproto::Reports::ReportList
   get all reports from `world.status.reports`

Building
--------

Copy this repository in dfhack's `plugins/external` directory. Add a `add_subdirectory` command for the corresponding sub-directory in `plugins/external/CMakeLists.txt` (if it does not exists DFHack's cmake script may create it automatically). Build the plugin using DFHack's scripts (see [DFHack documentation](https://docs.dfhack.org/en/latest/docs/dev/compile/index.html)).
