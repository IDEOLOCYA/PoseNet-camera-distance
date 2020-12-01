# PoseNet-camera-distance
Bu proje Tek bir kamera kullanarak önceden öğretilmiş basit bir cisme olan uzaklığı ve açıları yani 6 DoF'u yaklaşık 2cm ve 1.8 derece hata payı ile tespit edebilmektedir.
<br>Bu proje, benim ikinci zorunlu stajım esnasında BİTES te geliştirilmiştir.
<br><br>Sırasıyla yapılan işlemler şu şekildedir:
<ol>
<li>Charuco için gerekli imgeleri ve satranç tahtasını hazırladım. 
<li>Matlab yardımıyla Focal Length ve Distance Coefficient değerlerini satranç tahtası fotoğrafları ile kalibrasyon yapabilmek amacıyla bulup ardından yml uzantısıyla kaydettim.(Bu ve bundan sonraki aşamalarda görüntülerde hep no focus seçeneği ile ççalışılması gerekmektedir)</li>
<li>Charuco kütüphanesi vasıtasıyla kalibrasyonu test ettim. Verilerin doğruluğundan ve başlangıç noktasından emin oldum.</li>
<li>Charuco board'un gerekli olan çok küçük bir kısmı(4 imge içerir) haricini eğitim sırasında ezberlenmemesi için kapattım.</li>
<li>Hedef cismi referans noktasına yerleştirdikten sonra yaklaşık 7 dakikalık bir video kaydı aldım. Ardından bu video kaydı vasıtasıyla datasetimi oluşturum.</li>
<li>Oluşturulan datasetin içerisinde hala gözüken charuco imgelerini ezberlenmesini önlemek için kestim ve dataset içerisindeki görselleri tekrar boyutlandırdım.</li>
<li>Görsellerden elde ettiğim referans noktarına olan verileri kodunu kullandığım X beyin formatı için tekrar düzenledim.</li>
<li>Dataseti Google Colab yardımıyla yaklaşık 25 saat boyunca eğittim.</li>
<li>Pyqt yazılımı vasıtasıyla gerçek zamanlı olarak ağ üzerinden telefon kamerası vasıtasıyla öğretilmiş cisme olan uzaklığı içeren kodu yazdım. </li>
<li>Nihayetinde telefonun işlem gücü yetmediği için uzaktan telefon ile çekilen görüntülerin uzak ortamda bilgisayar üzerinden 6 DoF'u ölçmesini sağladım.</li>
</ol>


Eksiklik ve hatam ise bazı açılarda 6 DoF içerisinde bulunan row,yaw ve pitch verilerini son halde yanlış çizmesidir. Bunun nedeni eğitilen yapay zekamız her ne kadar quaternion olarak verileri doğru tahmin etmeyi başarsa bile Euler transformu esnasında eksenler karışmaktadır. Çözümü ise quternionu transform matrixlerle ve pixel olarak cismin odak noktasını bulup teker teker x’, y’, z’ ünü tespit edilmesidir. 
<br>Ne yazık ki staj süresince bu işlemi tamamlayamamış bulunmaktayım.

Zaman ayırıp okuduğunuz için teşekkür ederim.
