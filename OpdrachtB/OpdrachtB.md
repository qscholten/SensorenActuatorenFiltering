# Sensoren, Actuatoren en Filtering
***Qing Scholten***

<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script>
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({ tex2jax: {inlineMath: [['$', '$']]}, messageStyle: "none" });
</script>

1. $f_s = 9000 Hz$ en $N=256$. De hoogste frequentie die hier mee te meten is $_{max}=\frac{f_s}{2} = \frac{9000}{2}=4500Hz$. De minimale frequentie die te meten is is $f_{min}=\frac{f_s}{N}=\frac{9000}{256}=35,15625$.

2. 
```
  BeginMicro = micros();
  samples[sampleCounter] = (float32_t)analogRead(AUDIO_INPUT_PIN);
  EindMicro = micros();
  Serial.print(BeginMicro); Serial.print("-"); Serial.print(EindMicro); Serial.print("="); Serial.println(EindMicro-BeginMicro);
```

![alt text](image.png)
Een sample meting duurt 23 tot 24 microseconden.


3. Dit loopt niet knel want 23/24 microseconden is kleiner dan de periodetijd van een sampel wat $\frac{1}{9000}=111$ microseconden is. 

4. Er zit $T_s=111$ microseconden tussen twee samples.

5. Het duurt 29 microseconden = 0.029 miliseconden.
![alt text](image-1.png)
```
BeginMicro = micros();
    for (int t=0; t<256; t++) timeDomainRaw[t]=samples[2*t]; // Copy the samples into TimeDomain as the FFT uses samples as output
    EindMicro = micros();
    Serial.print(BeginMicro); Serial.print("-"); Serial.print(EindMicro); Serial.print("="); Serial.println(EindMicro-BeginMicro);
```

6. De frequentie resolutie is $\Delta f = \frac{f_s}{N}=\frac{9000}{256}=35.15625Hz$

7.
TijdsdomeinRaw
![alt text](image-2.png)
TijdsdomeinRev
![alt text](image-3.png)
Frequentiedomein
![alt text](image-4.png)
Er zijn in het frequentiedomein drie pieken te zien. De eerste piek is de 