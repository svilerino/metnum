function [STATS] = compare_video_same_method(video_prefix,method)
    interpolation_ratios = [1 2 5 10];

    %mse_f = figure;
    %hold all;
    %xlabel('Frame Interpolado','FontSize',10);
    %ylabel('ECM','FontSize',10);

    %psnr_f = figure;
    %hold all;
    %xlabel('Frame Interpolado','FontSize',10);
    %ylabel('PSNR','FontSize',10);

    for i = 1:size(interpolation_ratios,2)
        prefix = sprintf('%s%s%s%s%i',video_prefix,'-',method,'-k',interpolation_ratios(i));
        [MSE,PSNR] = video_vs_video(sprintf('%s%s',prefix,'.grayscale.avi'),sprintf('%s%s',prefix,'.avi'),interpolation_ratios(i));
        STATS(i,:) = [mean(MSE) std(MSE) max(MSE) min(MSE)];
        %figure(mse_f);
        %plot(1:size(MSE,2),MSE,'-');
        %figure(psnr_f);
        %plot(1:size(PSNR,2),PSNR,'-');
    end

    %legend('1','2','5','10');
    %figure(mse_f);
    %legend('1','2','5','10');

    %saveas(mse_f,sprintf('%s%s%s%i','mse_interpolation-',video_prefix,'-',method),'png');
    %saveas(psnr_f,sprintf('%s%s%s%i','psnr_interpolation-',video_prefix,'-',method),'png');
end
