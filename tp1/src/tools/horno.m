function horno(inputfile, solution, output_img_path, isofile)

ifid = fopen(inputfile);
solfid = fopen(solution);
instparam = fscanf(ifid, '%f',6);
if (nargin > 3)
   isofid = fopen(isofile);
end

angulos = instparam(4);
radios = instparam(3);
radioi = instparam(1);
radioe = instparam(2);
puntos = angulos*radios;
caloriso = instparam(5);
ninst = instparam(6);
deltaAng = 2*pi/angulos;
deltaRad = (radioe-radioi)/(radios-1);

for k = 1:ninst
    [pathstr, fileinname, ext] = fileparts(inputfile);

    out_heatmappath = strcat(output_img_path, fileinname, '_inst_', sprintf('%03d', k), '_heatmap.png');
    out_isomap = strcat(output_img_path, fileinname, '_inst_', sprintf('%03d', k), '_isomap.png');

    %armado de b 
    b = zeros(puntos,1);
    baux = fscanf(ifid,'%f',2*angulos);
    b(1:angulos) = baux(1:angulos);
    b(end-angulos+1:end) = baux(angulos+1:end);
    
    x = fscanf(solfid,'%f',puntos);
    
    xdib = zeros(puntos,1);
    ydib = zeros(puntos,1);

    pis = linspace(0,2*pi,angulos+1);
    pis = pis(1:angulos);

    for i=1:radios
        radAct = radioi + deltaRad*(i-1); 
        for j=1:angulos
           xdib(idx(i,j,angulos)) = cos(pis(j))*radAct;
           ydib(idx(i,j,angulos)) = sin(pis(j))*radAct;
        end
    end

    %Aca ploteo el calor en el horno
    h = figure;
    xdib = reshape(xdib,angulos,radios);
    xdib(angulos+1,:) = xdib(1,:);
    ydib = reshape(ydib,angulos,radios);
    ydib(angulos+1,:) = ydib(1,:);
    x = reshape(x,angulos,radios);
    x(angulos+1,:) = x(1,:);
    h = pcolor(xdib,ydib,x);
    title('Mapa de calor de la pared del horno')
    saveas(h, out_heatmappath)

    %aca ploteo la isoterma
    if (nargin > 3)
        radioiso = fscanf(isofid, '%f',angulos);
        radioiso = [radioiso; radioiso(1)];
        radioiso = radioiso';

        theta=linspace(0,2*pi,angulos+1);
        interno=linspace(radioi,radioi,angulos+1);
        externo=linspace(radioe,radioe,angulos+1);
        
        h = figure;
        title('Ubicacion de la isoterma en la pared del horno')
        polar(theta,interno), hold on;
        polar(theta,radioiso, 'r');
        polar(theta,externo);

        saveas(h, out_isomap)
        hold off;
        % pause;
    end
end
