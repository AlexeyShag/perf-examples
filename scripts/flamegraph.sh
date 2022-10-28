perf script | ./../FlameGraph/stackcollapse-perf.pl > perf.profdata
./../FlameGraph/flamegraph.pl perf.profdata > perf.svg
