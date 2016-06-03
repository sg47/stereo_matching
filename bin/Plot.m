yNoc0 = [50.5405 27.2766 14.1269 2.35754 5.69841; 47.8693 24.428 12.6087 2.55098 12.5429] ;
yOcc0 = [50.5405 27.2766 14.1269 2.35754 5.69841; 47.8693 24.428 12.6087 2.55098 12.5429] ;
sgbmNoc0 = [69.297 19.9719 6.45575 2.53466 1.74067; 69.2347 19.3772 6.61501 2.71179 2.06125] ;
sgbmOcc0 = [69.297 19.9719 6.45575 2.53466 1.74067; 69.2347 19.3772 6.61501 2.71179 2.06125] ;

x = [1:1:2] ; 
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
yNoc1 = [49.8687 27.2339 14.304 2.54688 6.04654; 46.3855 24.4134 12.9742 2.62408 13.6028] ;
yOcc1 = [49.8687 27.2339 14.304 2.54688 6.04654; 46.3855 24.4134 12.9742 2.62408 13.6028] ;
sgbmNoc1 = [69.297 19.9719 6.45575 2.53466 1.74067; 69.2347 19.3772 6.61501 2.71179 2.06125] ;
sgbmOcc1 = [69.297 19.9719 6.45575 2.53466 1.74067; 69.2347 19.3772 6.61501 2.71179 2.06125] ;

x = [1:1:2] ; 
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
