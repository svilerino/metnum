function [STATS] = compare_video_same_interpolation(video_prefix,interpolation)
    methods = ['VECINO';'LINEAL';'SPLINE'];

    mse_f = figure('Position',[0 0 800 600]);
    hold all;
    xlabel('Frame Interpolado','FontSize',10);
    ylabel('ECM','FontSize',10);

    psnr_f = figure('Position',[0 0 800 600]);
    hold all;
    xlabel('Frame Interpolado','FontSize',10);
    ylabel('PSNR','FontSize',10);

    for i=1:size(methods,1)
        prefix = sprintf('%s%s%s%s%i',video_prefix,'-',methods(i,:),'-k',interpolation);
        [MSE,PSNR] = video_vs_video(sprintf('%s%s',prefix,'.grayscale.avi'),sprintf('%s%s',prefix,'.avi'),interpolation);
        STATS(i,:) = [mean(MSE) std(MSE) max(MSE) min(MSE)];
        figure(mse_f);
        plot(1:size(MSE,2),MSE,'-');
        figure(psnr_f);
        plot(1:size(PSNR,2),PSNR,'-');
    end

    legend(methods);
    figure(mse_f);
    legend(methods);

    set(mse_f,'Units','Inches');
    pos = get(mse_f,'Position');
    set(mse_f,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
    print(mse_f,sprintf('%s%s%s%i','mse_methods-',video_prefix,'-k',interpolation),'-dpdf','-r600','-painters');

    set(psnr_f,'Units','Inches');
    pos = get(psnr_f,'Position');
    set(psnr_f,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
    print(psnr_f,sprintf('%s%s%s%i','psnr_methods-',video_prefix,'-k',interpolation),'-dpdf','-r600','-painters');
end
