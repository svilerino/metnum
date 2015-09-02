ffmpeg -framerate 3/1 -start_number 5 -i test5_%3d_radios_inst_001_heatmap.png -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -r 30 -pix_fmt yuv420p test5_video_heatmap.mp4 
ffmpeg -framerate 3/1 -start_number 5 -i test5_%3d_radios_inst_001_isomap.png -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -r 30 -pix_fmt yuv420p test5_video_isomap.mp4 

ffmpeg -framerate 3/1 -start_number 5 -i test6_%3d_radios_inst_001_heatmap.png -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -r 30 -pix_fmt yuv420p test6_video_heatmap.mp4 
ffmpeg -framerate 3/1 -start_number 5 -i test6_%3d_radios_inst_001_isomap.png -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -r 30 -pix_fmt yuv420p test6_video_isomap.mp4 
