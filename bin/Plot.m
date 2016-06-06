yNoc0 = [53.4614 25.1662 12.889 2.75549 5.72792; 54.7551 25.2983 12.7572 2.57772 4.6117] ;
yOcc0 = [53.4273 25.1501 12.8805 2.75375 5.78834; 54.709 25.2773 12.7464 2.57548 4.69187] ;
sgbmNoc0 = [69.5817 18.6666 6.5673 1.60459 3.57982; 70.1998 18.4822 6.35546 1.64634 3.31621] ;
sgbmOcc0 = [69.5372 18.6559 6.56194 1.60342 3.64163; 70.1408 18.468 6.3489 1.64494 3.39742] ;

x = [1:1:2] ; 
 figure() ; 
 subplot(2,1,1); 
 bar(x,yOcc0);
 grid on ; 
title('CannyEdgeOcc NonAdaptive');

ax=gca; 
 ax.XTickLabel = { '16Edge','16PointAl'} 
ax.XTickLabelRotation=45;
subplot(2,1,2); 
 bar(x,sgbmOcc0);
 grid on ; 
title('SGBMOcc NonAdaptive');

ax=gca; 
 ax.XTickLabel = { '16Edge','16PointAl'} 
ax.XTickLabelRotation=45;
yNoc1 = [45.7914 21.9672 12.8929 4.24655 15.102; 46.5736 22.4036 13.7023 4.26027 13.0602] ;
yOcc1 = [45.7643 21.9533 12.8832 4.24305 15.1561; 46.5355 22.3851 13.6901 4.25603 13.1332] ;
sgbmNoc1 = [69.5817 18.6666 6.5673 1.60459 3.57982; 70.1998 18.4822 6.35546 1.64634 3.31621] ;
sgbmOcc1 = [69.5372 18.6559 6.56194 1.60342 3.64163; 70.1408 18.468 6.3489 1.64494 3.39742] ;

x = [1:1:2] ; 
 figure() ; 
 subplot(2,1,1); 
 bar(x,yOcc1);
 grid on ; 
title('CannyEdgeOcc Adaptive');

ax=gca; 
 ax.XTickLabel = { '16Edge','16PointAl'} 
ax.XTickLabelRotation=45;
subplot(2,1,2); 
 bar(x,sgbmOcc1);
 grid on ; 
title('SGBMOcc  Adaptive');

ax=gca; 
 ax.XTickLabel = { '16Edge','16PointAl'} 
ax.XTickLabelRotation=45;
