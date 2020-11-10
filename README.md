Online algorithm filters
========================

The only one working here right now is for statistical moments.

The `moments` command reads the entire set and reports the std dev,
kurtosis and skew.  The `injest` command produces the internal streaming
moments - so the input data can be shared and each injest command
processes a single shard.  The `merge` command reads a series of
injest command outputs and merges them to a single vector of the
internal moments.  The `derived` command reads the merge output
(or the output of a single shard) and reports the std dev,
kurtosis and skew.

See demo scripts in the `bin/` directory.
