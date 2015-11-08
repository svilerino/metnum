function [STATS,DIFS] = compare_video_spline(video_prefix,interpolation)
    blk_sizes = [4 8 16 32];

    mse_f = figure('Position',[0 0 800 600]);
    hold all;
    xlabel('Frame Interpolado','FontSize',10);
    ylabel('ECM','FontSize',10);

    psnr_f = figure('Position',[0 0 800 600]);
    hold all;
    xlabel('Frame Interpolado','FontSize',10);
    ylabel('PSNR','FontSize',10);

    prefix = sprintf('%s%s%i',video_prefix,'-SPLINE-k',interpolation);
    %for i=1:size(blk_sizes,2)
    [MSE4,PSNR4] = video_vs_video(sprintf('%s%s',prefix,'.grayscale.avi'),sprintf('%s%s%i%s',prefix,'-blk',4,'.avi'),interpolation);
    STATS(1,:) = [mean(MSE4) std(MSE4) max(MSE4) min(MSE4)];
    figure(mse_f);
    plot(1:size(MSE4,2),MSE4,'-');
    figure(psnr_f);
    plot(1:size(PSNR4,2),PSNR4,'-');

    [MSE8,PSNR8] = video_vs_video(sprintf('%s%s',prefix,'.grayscale.avi'),sprintf('%s%s%i%s',prefix,'-blk',8,'.avi'),interpolation);
    STATS(2,:) = [mean(MSE8) std(MSE8) max(MSE8) min(MSE8)];
    figure(mse_f);
    plot(1:size(MSE8,2),MSE8,'-');
    figure(psnr_f);
    plot(1:size(PSNR8,2),PSNR8,'-');

    [MSE16,PSNR16] = video_vs_video(sprintf('%s%s',prefix,'.grayscale.avi'),sprintf('%s%s%i%s',prefix,'-blk',16,'.avi'),interpolation);
    STATS(3,:) = [mean(MSE16) std(MSE16) max(MSE16) min(MSE16)];
    figure(mse_f);
    plot(1:size(MSE16,2),MSE16,'-');
    figure(psnr_f);
    plot(1:size(PSNR16,2),PSNR16,'-');

    [MSE32,PSNR32] = video_vs_video(sprintf('%s%s',prefix,'.grayscale.avi'),sprintf('%s%s%i%s',prefix,'-blk',32,'.avi'),interpolation);
    STATS(4,:) = [mean(MSE32) std(MSE32) max(MSE32) min(MSE32)];
    figure(mse_f);
    plot(1:size(MSE32,2),MSE32,'-');
    figure(psnr_f);
    plot(1:size(PSNR32,2),PSNR32,'-');

    [MSE,PSNR] = video_vs_video(sprintf('%s%s',prefix,'.grayscale.avi'),sprintf('%s%s',prefix,'.avi'),interpolation);
    STATS(5,:) = [mean(MSE) std(MSE) max(MSE) min(MSE)];
    figure(mse_f);
    plot(1:size(MSE,2),MSE,'-');
    figure(psnr_f);
    plot(1:size(PSNR,2),PSNR,'-');

    legend('4','8','16','32','Todo');
    figure(mse_f);
    legend('4','8','16','32','Todo');

    %saveas(mse_f,sprintf('%s%s%s%i','mse_spline-',video_prefix,'-k',interpolation),'png');
    %saveas(psnr_f,sprintf('%s%s%s%i','psnr_spline-',video_prefix,'-k',interpolation),'png');
    set(mse_f,'Units','Inches');
    pos = get(mse_f,'Position');
    set(mse_f,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
    print(mse_f,sprintf('%s%s%s%i%s','mse_spline-',video_prefix,'-k',interpolation,'.pdf'),'-dpdf','-painters','-r600');

    set(psnr_f,'Units','Inches');
    pos = get(psnr_f,'Position');
    set(psnr_f,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
    print(psnr_f,sprintf('%s%s%s%i%s','psnr_spline-',video_prefix,'-k',interpolation,'.pdf'),'-dpdf','-painters','-r600');

    DIFS = zeros(5);
    DIFS(1,2) = max(MSE4-MSE8);
    DIFS(1,3) = max(MSE4-MSE16);
    DIFS(1,4) = max(MSE4-MSE32);
    DIFS(1,5) = max(MSE4-MSE);

    DIFS(2,1) = max(MSE8-MSE4);
    DIFS(2,3) = max(MSE8-MSE16);
    DIFS(2,4) = max(MSE8-MSE32);
    DIFS(2,5) = max(MSE8-MSE);

    DIFS(3,1) = max(MSE16-MSE4);
    DIFS(3,2) = max(MSE16-MSE8);
    DIFS(3,4) = max(MSE16-MSE32);
    DIFS(3,5) = max(MSE16-MSE);

    DIFS(4,1) = max(MSE32-MSE4);
    DIFS(4,2) = max(MSE32-MSE8);
    DIFS(4,3) = max(MSE32-MSE16);
    DIFS(4,5) = max(MSE32-MSE);

    DIFS(5,1) = max(MSE-MSE4);
    DIFS(5,2) = max(MSE-MSE8);
    DIFS(5,3) = max(MSE-MSE16);
    DIFS(5,4) = max(MSE-MSE32);
end
