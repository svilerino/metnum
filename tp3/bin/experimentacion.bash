#!/bin/bash

for video in `ls *.$1`; do
    for iframes in 1 2 5 10; do
        ./tp3 ./$video ./${video%.*}-VECINO-k${iframes}.avi 0 $iframes >${video%.*}-VECINO-k${iframes}.txt 2>&1
        mv ${video%.*}.grayscale.avi ${video%.*}-VECINO-k${iframes}.grayscale.avi
        ./tp3 ./$video ./${video%.*}-LINEAL-k${iframes}.avi 1 $iframes >${video%.*}-LINEAL-k${iframes}.txt 2>&1
        mv ${video%.*}.grayscale.avi ${video%.*}-LINEAL-k${iframes}.grayscale.avi

        #for blk_size in 4 8 12 16; do
        #    ./tp3 ./$video ./${video%.*}-SPLINE-k${iframes}-blk${blk_size}.avi 2 $iframes $blk_size >${video%.*}-SPLINE-k${iframes}-blk${blk_size}.txt 2>&1
        #    mv ${video%.*}.grayscale.avi ${video%.*}-SPLINE-k${iframes}-blk${blk_size}.grayscale.avi
        #done
    done
done
