

// 地图类型
var mapType = {
    localGoogleSatellite  : 0,
    remoteGoogleSatellite : 1,
    localGoogleStreet     : 2,
    remoteGoogleStreet    : 3
}


// 全局变量
var g_arg = {
    obj     : null,
    map     : null,
    street  : null,
    type    : mapType.localGoogleSatellite,
    center  : [28.1128547,112.8668242],
    zoom    : 17,
    minZoom : 3,
    maxZoom : 22,
    group   : null
}


// 初始化地图
function mapInit(){
    var url = "map/s/{z}/{x}/{y}.jpg";
//    g_arg.type = mapType.remoteGoogleStreet;
    if(g_arg.type == mapType.localGoogleSatellite){
        url = "map/s/{z}/{x}/{y}.jpg";
    }else if(g_arg.type == mapType.localGoogleStreet){
        url = "map/m/{z}/{x}/{y}.jpg";
    }else if(g_arg.type == mapType.remoteGoogleSatellite){
        url = "http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}";
    }else if(g_arg.type == mapType.remoteGoogleStreet){
        url = "http://{s}.google.com/vt/lyrs=m&x={x}&y={y}&z={z}";
    }
    
    if(g_arg.map == null){
        g_arg.map = L.map('googleMap',{zoomAnimation:true});
        g_arg.map.setView(g_arg.center,g_arg.zoom);
        g_arg.map.on("move",function(event){
            var b = g_arg.map.getBounds();
            var zoom = g_arg.map.getZoom();
            if(g_arg.obj != null){
                g_arg.obj.getBounds(b.getSouth(),b.getNorth(),b.getWest(),b.getEast(),zoom,g_arg.type);
            }
        })
    }
    if(g_arg.map.hasLayer(g_arg.street))g_arg.map.removeLayer(g_arg.street);
    g_arg.street = L.tileLayer(url,{ minZoom: g_arg.minZoom,maxZoom: g_arg.maxZoom,subdomains:['mt0','mt1','mt2','mt3']});
    g_arg.street.addTo(g_arg.map);
	if(g_arg.group == null){
		g_arg.group = L.markerClusterGroup();
		g_arg.map.addLayer(g_arg.group);
	}
}


// 测试用的marker
function add_marker(lat,lng){
    lat = lat.toFixed(7);
    lng = lng.toFixed(7);
    var marker = L.marker([lat,lng]).addTo(g_arg.map);
	marker.dragging.enable();
    marker.bindPopup("lat : "+lat+"</br>lng : "+lng);
    marker.on('dragend',function(event){
        var pt = marker.getLatLng();
        var lat_new = pt.lat.toFixed(7);
        var lng_new = pt.lng.toFixed(7);
        marker.setPopupContent("lat : "+lat_new+"</br>lng : "+lng_new);
	});
}

