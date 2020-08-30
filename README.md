# Completely Fair Scheduler

This repo implements std::multimap using LLRB and simulate CFS with this multimap. The testing for LLRB is done through Googletest with unit testing inside the Test directory. CFS requires input file with each line having the format: <name of running task> <start time> <end time> and outputs the task running in order.
