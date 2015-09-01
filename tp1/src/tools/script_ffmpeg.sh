ffmpeg -framerate 4/1 -i test5_%*_radios_inst_1_isomap.png -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -r 30 -pix_fmt yuv420p test5_video_isomap.mp4 
ffmpeg -framerate 4/1 -i test5_%*_radios_inst_1_heatmap.png -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -r 30 -pix_fmt yuv420p test5_video_heatmap.mp4 

ffmpeg -framerate 7/1 -i test5_%*_radios_inst_1_isomap.png -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -r 30 -pix_fmt yuv420p test5_video_isomap_fast.mp4 
ffmpeg -framerate 7/1 -i test5_%*_radios_inst_1_heatmap.png -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -r 30 -pix_fmt yuv420p test5_video_heatmap_fast.mp4 