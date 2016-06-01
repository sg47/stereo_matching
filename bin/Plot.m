yNoc = [32.7727 21.885 20.2559 9.34796 15.7384; 31.5644 20.8306 21.1284 9.67488 16.8018; 32.6758 21.2988 20.1169 9.03011 16.8784] ;
yOcc = [32.7727 21.885 20.2559 9.34796 15.7384; 31.5644 20.8306 21.1284 9.67488 16.8018; 32.6758 21.2988 20.1169 9.03011 16.8784] ;
sgbmNoc = [68.3282 22.531 5.25696 1.6169 2.26691; 67.96 22.5885 5.41032 1.65867 2.38253; 68.1486 22.6867 5.20693 1.66555 2.29223] ;
sgbmOcc = [68.3282 22.531 5.25696 1.6169 2.26691; 67.96 22.5885 5.41032 1.65867 2.38253; 68.1486 22.6867 5.20693 1.66555 2.29223] ;

x = [1:1:3] ; 
 figure() ; 
 subplot(2,1,1); 
 bar(x,yNoc);
 grid on ; 
title('CannyEdgeNoc');

ax=gca; 
 ax.XTickLabel = {'1Edge', '2Edge','4Edge','8Edge','12Edge', '16Edge','16PointAl','16PointDia','12Point', '8PointAl','4PointDia','2point','1point'} 
ax.XTickLabelRotation=45;
subplot(2,1,2); 
 bar(x,sgbmNoc);
 grid on ; 
title('SGBMNoc');

ax=gca; 
 ax.XTickLabel = {'1Edge', '2Edge','4Edge','8Edge','12Edge', '16Edge','16PointAl','16PointDia','12Point', '8PointAl','4PointDia','2point','1point'} 
ax.XTickLabelRotation=45;
 figure() ; 
 subplot(2,1,1); 
 bar(x,yOcc);
 grid on ; 
title('CannyEdgeOcc');

ax=gca; 
 ax.XTickLabel = {'1Edge', '2Edge','4Edge','8Edge','12Edge', '16Edge','16PointAl','16PointDia','12Point', '8PointAl','4PointDia','2point','1point'} 
ax.XTickLabelRotation=45;
subplot(2,1,2); 
 bar(x,sgbmOcc);
 grid on ; 
title('SGBMOcc');

ax=gca; 
 ax.XTickLabel = {'1Edge', '2Edge','4Edge','8Edge','12Edge', '16Edge','16PointAl','16PointDia','12Point', '8PointAl','4PointDia','2point','1point'} 
ax.XTickLabelRotation=45;
