function [MSE,PSNR] = video_vs_video(video1_filename, video2_filename, interpol_ratio)
    obj1 = mmreader(video1_filename);
    obj2 = mmreader(video2_filename);
    video1 = obj1.read();
    video2 = obj2.read();
    n_frames = min(obj1.NumberOfFrames,obj2.NumberOfFrames);
    interpol_blk_size = interpol_ratio+1;
    interpolated_frame=1;
    for interpol_blk_idx=0:(n_frames/interpol_blk_size)-1
        for frame_idx=interpol_blk_idx*interpol_blk_size+2:interpol_blk_idx*interpol_blk_size+interpol_blk_size
            frame1 = video1(:,:,:,frame_idx);
            frame2 = video2(:,:,:,frame_idx);
            MSE(interpolated_frame) = mean2((frame1-frame2).^2);
            PSNR(interpolated_frame) = 10*log10(65025/MSE(interpolated_frame));
            interpolated_frame=interpolated_frame+1;
        end
    end
end
