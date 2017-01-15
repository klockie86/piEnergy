<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
<title>Klockhuis</title>
<link rel="icon" type="image/x-icon" href="favicon.ico">
<script type="text/javascript" src="js/jquery-2.0.2.min.js"></script>
<script src="js/highstock.js"></script>
<script type="text/javascript" src="js/jquery.mobile-1.4.2.min.js"></script>
<link rel="stylesheet" href="js/jquery.mobile-1.4.2.min.css" >
</head>
<?php
$link = mysql_connect('localhost', 'user', 'pass');
if (!$link) {
    die('Could not connect: ' . mysql_error());
}

mysql_select_db("databasename");

$result = mysql_query("SELECT datetime, pulses from pulspermin_gas WHERE id > 1");

mysql_close($link);

while ($row = mysql_fetch_array($result, MYSQL_NUM)) {

        $date = date_create($row[0]);
        $format = $date->format('Y, m-1, d, H, i, s');

        $data[] = "[Date.UTC($format), $row[1]]";
}
?>
<div data-role="navbar">
        <ul>
                <li><a data-ajax="false" href="http://192.168.1.10">verlichting</a></li>
                <li><a data-ajax="false" href="http://192.168.1.10:8080/gas.php">gas</a></li>
                <li><a data-ajax="false" href="http://192.168.1.10:8080/kwh.php">kwh</a></li>
                <li><a data-ajax="false" href="http://192.168.1.10:8080/water.php">water</a></li>
        </ul>
</div><!-- /navbar -->

<script type="text/javascript">
$(function () {
   $('#container').highcharts('StockChart', {
        chart: {
        },
        colors: ['#999900'],
        title:{
            text:'Gas verbruik',
            x:-20 //center
        },
    xAxis: {
        type: 'datetime', //ensures that xAxis is treated as datetime values

        },


        series: [{
                name : 'Data',
                data: [<?php echo join($data, ',') ?>],
        }]
    });
});
</script>
<div id="container" style="width:100%; height:500px;"></div>
</html>
