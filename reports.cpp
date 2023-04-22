/*
 * Copyright (c) 2023 Clement Vuchener
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 */

#include "PluginManager.h"
#include "RemoteServer.h"

#include "df/world.h"
#include "df/report.h"

#include "Reports.pb.h"

using namespace DFHack;
using namespace dfproto::Reports;

DFHACK_PLUGIN("Reports");
REQUIRE_GLOBAL(world);

DFhackCExport command_result plugin_init(color_ostream &, std::vector<PluginCommand> &)
{
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown(color_ostream &)
{
    return CR_OK;
}

static void init_report(const df::report *df_report, Report *report)
{
    using announcement_type_traits = df::enum_traits<df::announcement_type>;
    report->set_id(df_report->id);
    if (announcement_type_traits::is_valid(df_report->type))
        report->set_type(announcement_type_traits::key_table[df_report->type]);
    report->set_text(DF2UTF(df_report->text));
    report->set_color(df_report->color);
    report->set_bright(df_report->bright);
    report->set_continuation(df_report->flags.bits.continuation);
    report->set_repeat(df_report->repeat_count);
    report->set_year(df_report->year);
    report->set_time(df_report->time);
}

static void init_report_list(const std::vector<df::report *> &df_reports, ReportList *report_list)
{
    auto reports = report_list->mutable_reports();
    reports->Clear();
    reports->Reserve(df_reports.size());
    for (const auto *df_report: df_reports) {
        init_report(df_report, reports->Add());
    }
}

static command_result get_announcements(color_ostream &, const EmptyMessage *, ReportList *out)
{
    init_report_list(world->status.announcements, out);
    return CR_OK;
}

static command_result get_reports(color_ostream &, const EmptyMessage *, ReportList *out)
{
    init_report_list(world->status.reports, out);
    return CR_OK;
}

DFhackCExport RPCService *plugin_rpcconnect(color_ostream &)
{
    RPCService *svc = new RPCService();
    svc->addFunction("GetAnnouncements", get_announcements, SF_ALLOW_REMOTE);
    svc->addFunction("GetReports", get_reports, SF_ALLOW_REMOTE);
    return svc;
}
