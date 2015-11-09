function stats_bar_graphs(filename,B)
    %labels = {'blk 4','blk 8','blk 16','blk 32','entero'};
    labels = {'1','2','5','10'};
    fig = figure('Position',[0 0 800 600]);
    %hold on;
    %bar(1,B(1),'b');
    %bar(2,B(2),'r');
    %bar(3,B(3),'y');
    %bar(4,B(4),'g');
    bar(B)
    xlabel('Frames Interpolado','FontSize',20);
    set(gca,'XTick',1:5,'XTickLabel',labels);
    %legend('1','2','5','10','Location','northwest')
    legend('Vecino','Lineal','Spline','Location','northwest')
    set(fig,'Units','Inches');
    pos = get(fig,'Position');
    set(fig,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)]);
    print(fig,filename,'-dpdf','-painters','-r600');
end
