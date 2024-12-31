# Sensoren, Actuatoren en Filtering
***Qing Scholten***

<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script>
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({ tex2jax: {inlineMath: [['$', '$']]}, messageStyle: "none" });
</script>

1.

![alt text](image.png)
De blauwe lijn is het ingangssignaal en de groene lijn is het uitgangssignaal. Door de vorm van het uitgangssignaal is te zien dat het een laagdoorlaatfilter is.

2.

De differentievergelijking van het laagdoorlaatfilter is: 
$Y[n] = 0.15*X[n] + 0.85*Y[n-1]$

De differentievergelijking van het hoogdoorlaatfilter is:
$Y[n]=X[n]-X[n-1]+C*Y[n-1]$

3.

Onderstaande is de grafiek behorende bij het hoogdoorlaatfilter.
![alt text](image-1.png)
```arduino
float C = 0.95123;
Y[n] = X[n]-X[n-1]+C*Y[n-1];
```

4.

![alt text](image-2.png)

36.8% van de beginwaarde is $0.368*10 = 3.68$. 

Bij 20 is de waarde 36.8%. $\tau =  T * 20 = \frac{1}{f_s}*20=\frac{1}{10000}*20 = \frac{1}{500}$. Hierdoor is $f_c = \frac{1}{\tau}=\frac{1}{\frac{1}{500}}=500$. Dus de kantelfrequentie van dit filter is 500 Hz.

5.

![alt text](image-3.png)
![alt text](image-4.png)
![alt text](image-5.png)

Het is karakteristiek van een laagdoorlaatfilter aangezien de groene lijn het meest correspondeert met de blauwe lijn bij de lage frequenties ten opzichte van de hoge frequenties. Hoe hoger de frequentie, hoe minder de blauwe lijn met de groene lijnt correspondeert, dus hoe meer het signaal gedempt wordt. Bij een lage frequentie, zie bovenste plaatje, ligt de groene lijn bijna op de blauwe lijn, terwijl met de hoogste frequentie, zie onderste plaatje, er een groot verschil tussen de lijnen is.

6.

![alt text](image-7.png)

$X_{max}=10$ en $Y_{max} = 0.707*X_{max} = 0.707*10=7.07$ zoals te zien is op de afbeelding. Er worden in totaal 500 samples genomen en daarin zijn ongeveer 34 golven te zien. Dit betekent dat er per golf ongeveer $500/34=14.71$ samples zijn. Dit betekent dat met een sampletijd van $T_s = \frac{1}{f_s}=\frac{1}{10000} = 0.0001s$ een golf tijd  $T_{golf}=0.0001*14.71=0.001471s$ is. Dus de frequentie is $f_c = \frac{1}{T_{golf}} = \frac{1}{0.001471} = 679.81$ Hz.

7.

Het uitgangssignaal Yn loopt achter op het ingangssignaal Xn.

8.

Dit is de karakteristiek van een hoogdoorlaatfilter. Hierbij worden de lage frequenties gedempt, zoals te zien is in de onderstaande grafieken.

![alt text](image-8.png)
![alt text](image-9.png)
![alt text](image-10.png)

9.

![alt text](image-11.png)

Er worden in totaal 500 samples genomen en daarin zijn ongeveer 25 golven te zien. Dit betekent dat er per golf ongeveer $500/25=20$ samples zijn. Dit betekent dat de sampletijd van $T_s=\frac{1}{f_s}=\frac{1}{10000} = 0.0001s$ een golf tijd $T_{golf} = 0.0001 * 20 = 0.002s$ is. Dus de frequentie is $f_c = \frac{1}{T_{golf}} = \frac{1}{0.002} = 500$ Hz.

10.

Het uitgangssignaal loopt achter op het ingangssignaal.

11. 

![alt text](image-12.png)

12.

![alt text](image-13.png)

Ymax is 51. Er zijn in totaal 500 samples genomen. Daarin zijn ongeveer 15 golven te zien. Dit betekent dat er per golf ongeveer $500/15=33.3$ samples zijn. Dit betekent dat de tijd per golf $T_{golf} = 0.0001*33.3 = 0.00333s$ is. Dus de frequentie is $f = \frac{1}{T_{golf}} = \frac{1}{0.00333} = 300$ Hz.