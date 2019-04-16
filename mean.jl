#!/opt/local/bin/julia

# include("bin/mean.jl")

using DelimitedFiles
using Statistics
using StatsBase
using Distributions
using JSON
using Plots

data = readdlm("sample.ssv", ' ');
rates = Array{Float64}( data[:,5] )

µ = Statistics.mean( rates )
σ = Statistics.std( rates )
γ = StatsBase.skewness( rates, µ )
κ = StatsBase.kurtosis( rates, µ )

z = StatsBase.zscore( rates, µ, σ )
y = map( x -> round(x,digits=1), z )

println( "sample µ $µ  σ $σ  γ $γ  κ $κ" )

dto = Dict(
       "mean" => µ,
       "std" => σ,
       "data" => z,
      )

open( "out.json", "w" ) do io
    write( io, JSON.json(dto) )
end

d = Distributions.fit( Normal, rates )
println( d )

# Plots.histogram( z )

# h = StatsBase.fit( Histogram, z )
# Plots.plot( h )
