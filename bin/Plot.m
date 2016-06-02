yNoc0 = [50.0058 27.2196 14.5386 2.47079 5.76519; 47.3574 24.3064 13.0948 2.23422 13.0071] ;
yOcc0 = [50.0058 27.2196 14.5386 2.47079 5.76519; 47.3574 24.3064 13.0948 2.23422 13.0071] ;
sgbmNoc0 = [68.8843 20.1986 6.43692 2.45327 2.02687; 68.5116 19.8043 6.77691 2.62572 2.28147] ;
sgbmOcc0 = [68.8843 20.1986 6.43692 2.45327 2.02687; 68.5116 19.8043 6.77691 2.62572 2.28147] ;

x = [1:1:13] ; 
 figure() ; 
 subplot(2,1,1); 
 bar(x,yOcc0);
 grid on ; 
title('CannyEdgeOcc0');

ax=gca; 
 ax.XTickLabel = {'1Edge', '2Edge','4Edge','8Edge','12Edge', '16Edge','16PointAl','16PointDia','12Point', '8PointAl','4PointDia','2point','1point'} 
ax.XTickLabelRotation=45;
subplot(2,1,2); 
 bar(x,sgbmOcc0);
 grid on ; 
title('SGBMOcc0');

ax=gca; 
 ax.XTickLabel = {'1Edge', '2Edge','4Edge','8Edge','12Edge', '16Edge','16PointAl','16PointDia','12Point', '8PointAl','4PointDia','2point','1point'} 
ax.XTickLabelRotation=45;
yNoc1 = [49.0537 26.8166 14.8072 2.79206 6.53037; 46.8984 23.6585 12.6291 2.55822 14.2558] ;
yOcc1 = [49.0537 26.8166 14.8072 2.79206 6.53037; 46.8984 23.6585 12.6291 2.55822 14.2558] ;
sgbmNoc1 = [68.8843 20.1986 6.43692 2.45327 2.02687; 68.5116 19.8043 6.77691 2.62572 2.28147] ;
sgbmOcc1 = [68.8843 20.1986 6.43692 2.45327 2.02687; 68.5116 19.8043 6.77691 2.62572 2.28147] ;

x = [1:1:13] ; 
 figure() ; 
 subplot(2,1,1); 
 bar(x,yOcc1);
 grid on ; 
title('CannyEdgeOcc1');

ax=gca; 
 ax.XTickLabel = {'1Edge', '2Edge','4Edge','8Edge','12Edge', '16Edge','16PointAl','16PointDia','12Point', '8PointAl','4PointDia','2point','1point'} 
ax.XTickLabelRotation=45;
subplot(2,1,2); 
 bar(x,sgbmOcc1);
 grid on ; 
title('SGBMOcc1');

ax=gca; 
 ax.XTickLabel = {'1Edge', '2Edge','4Edge','8Edge','12Edge', '16Edge','16PointAl','16PointDia','12Point', '8PointAl','4PointDia','2point','1point'} 
ax.XTickLabelRotation=45;
