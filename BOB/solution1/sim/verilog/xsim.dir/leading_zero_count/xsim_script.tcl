set_param project.enableReportConfiguration 0
load_feature core
current_fileset
xsim {leading_zero_count} -view {{leading_zero_count_dataflow_ana.wcfg}} -tclbatch {leading_zero_count.tcl} -protoinst {leading_zero_count.protoinst}
