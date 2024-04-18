# 2023 EE6470 HW1
Part1 median and mean filter with FIFO 

Part2 median and mean filter with FIFO (using a 18-pixels data buffer)

本作業分為兩個部分。第一部分(mmfilter_FIFO)使用 FIFO channels 實現 median and
mean filter ， 一 次 只 在 filter 中 處 理 一 個 pixel 的 計 算 ； 而 第二部分
(mmfilter_FIFO_databuffer)基於第一部分的功能，加入了 18-pixels 的 data buffer，
表示一次傳送 18-pixels 的資料進入 filter，並在之後的計算直接使用 buffer 儲存的
資料來加快運算。
