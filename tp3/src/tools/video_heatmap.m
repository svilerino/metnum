function video_heatmap(vorg_filename, vint_filename, output_prefix)
    obj1 = mmreader(vorg_filename);
    obj2 = mmreader(vint_filename);
    vorg = obj1.read();
    vint = obj2.read();
    n_frames = min(obj1.NumberOfFrames,obj2.NumberOfFrames);

    %writerObj = VideoWriter('heatmap.avi');
    %writerObj.FrameRate = 60;
    %open(writerObj);
    for f=1:n_frames
        frame_org = vorg(:,:,:,f);
        frame_int = vint(:,:,:,f);
        frame = rgb2gray(abs(frame_org-frame_int));

        fig = figure('units','normalized','outerposition',[0 0 1 1],'visible','off');
        orig = subplot(1,3,1);
        image(frame_org);
        axis image;
        set(gca,'XTickLabel',[],'XTick',[]);
        set(gca,'YTickLabel',[],'YTick',[]);
        xlabel('Original');

        interpol = subplot(1,3,2);
        image(frame_int);
        axis image;
        set(gca,'XTickLabel',[],'XTick',[]);
        set(gca,'YTickLabel',[],'YTick',[]);
        xlabel('Interpolado');

        map = subplot(1,3,3);
        colormap(flipud(gray));
        image(frame);
        pos = get(gca,'position');
        axis image;
        colorbar;
        set(gca,'position',[pos(1) pos(2) pos(3) pos(4)]);
        set(gca,'XTickLabel',[],'XTick',[]);
        set(gca,'YTickLabel',[],'YTick',[]);
        xlabel('Diferencia');

        p = get(orig,'pos');p(3) = p(3) + 0.05;set(orig,'pos',p);
        p = get(interpol,'pos');p(3) = p(3) + 0.05;set(interpol,'pos',p);
        p = get(map,'pos');p(3) = p(3) + 0.05;set(map,'pos',p);

        set(fig,'Units','Inches');
        pos = get(fig,'Position');
        set(fig,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
        print(fig,sprintf('%s%s%03d',output_prefix,'_',f),'-dpng','-r300');
        %video_frame = getframe(fig);
        %orig_mode = get(fig, 'PaperPositionMode');
        %set(fig, 'PaperPositionMode', 'auto');
        %cdata = hardcopy(fig, '-Dzbuffer', '-r0');
        %set(fig, 'PaperPositionMode', orig_mode);
        %currFrame = im2frame(cdata);
        %writeVideo(writerObj, currFrame);
    end
    %close(writerObj);
end
