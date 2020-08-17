

// 全局变量
var g_map = null;
var g_group = null;
var g_street = null;
var g_center = [28.1128547,112.8668242];
var g_zoom = 17;
var g_minZoom = 3;
var g_maxZoom = 24;

var g_lng;
var g_lat;

var g_index = 0;
var g_layers = [];

var g_arrow_icon = L.icon({ iconUrl:'style/image/arrow.png',iconSize: [15, 15]});


// 测试用的marker
function add_marker(lat,lng){
    lat = lat.toFixed(7);
    lng = lng.toFixed(7);
    var marker = L.marker([lat,lng]).addTo(g_map);
	marker.dragging.enable();
    marker.bindPopup("lat : "+lat+"</br>lng : "+lng);
    marker.on('dragend',function(event){
        var pt = marker.getLatLng();
        var lat_new = pt.lat.toFixed(7);
        var lng_new = pt.lng.toFixed(7);
        marker.setPopupContent("lat : "+lat_new+"</br>lng : "+lng_new);
	});
    marker.source = "map";
    marker.id = g_index++;
    g_layers.push(marker);
}


// 添加车的图标
function add_car(lat,lng,angle,id){
    var index = -1;
    for(i=0;i<g_layers.length;i++){
        var temp = g_layers[i];
        if( (temp.source == "car") && (temp.id == id)){index=i;break;}
	}
	if(index == -1){
        var car_icon = L.icon({ iconUrl:'style/image/car.png',iconSize: [15, 15]});
        var marker = L.marker([lat,lng],{icon:car_icon,rotationAngle:angle,rotationOrigin:'center'}).addTo(g_map);
        marker.id = g_index++;
        marker.source = "car";
        g_layers.push(marker);
	}else{
        g_layers[index].setLatLng([lat,lng]);
        g_layers[index].setRotationAngle(angle);
    }
}


function get_obs_icon(id,source)
{
	var png = "style/image/obs_vision.png";
	if(source == "fusion")png = "style/image/obs_fusion.png";
	var icon = L.icon({ iconUrl:png,iconSize: [20, 20]});
	return icon;
}

// 添加障碍的图标
function add_obs(lat,lng,id,source)
{
    var index = -1;
    var marker = null;
	var myicon = get_obs_icon(id,source);
    lat = lat.toFixed(7);
    lng = lng.toFixed(7);
    for(i=0;i<g_layers.length;i++){
        var temp = g_layers[i];
        if( (temp.source == source) && (temp.id == id)){index=i;break;}
    }
    if(index == -1){
        marker = L.marker([lat,lng],{icon:myicon}).addTo(g_map);
        marker.bindPopup("");
        marker.source = source;
        marker.id = id;
        g_layers.push(marker);
    }else{
        marker = g_layers[index];
        marker.setLatLng([lat,lng]);
    }
    marker.setPopupContent("id : "+id+"</br>lat : "+lat+"</br>lng : "+lng);
}

//清除特定source的地图控件
function clear_obs(source){
    var len = g_layers.length , i;
    for(i=len-1;i>=0;i--){
        if(g_layers[i].source != source)continue;
        g_map.removeLayer(g_layers[i]);
        g_layers.splice(i,1);
    }
}

// 显示障碍物
function qt_show_obs(str)
{
	var json = JSON.parse(str);
    clear_obs(json.source);
	for(var i in json.array){
		var o = json.array[i];
        add_obs(o.lat*1e-7,o.lng*1e-7,o.id,json.source);
	}
}

// 清除地图控件
function qt_clear()
{
    var len = g_layers.length , i;
    for(i=0;i<len;i++){
        g_map.removeLayer(g_layers[i]);
    }
    g_group.clearLayers();
    g_layers.splice(0,len);
    g_index = 0;
}

// 在中心位置添加marker
function qt_add()
{
    var center = g_map.getCenter();
    add_marker(center.lat,center.lng);
}

// 在目标位置添加marker
function qt_addat(lat,lng)
{
    var center = [lat,lng];
    add_marker(lat,lng);
    g_map.panTo(center,{animate:true,duration:0.5});
}

// 视野转移到目标位置
function qt_go(lat,lng)
{
    var center = [lat,lng];
    g_map.panTo(center,{animate:true,duration:0.5});
}

// 初始化地图
function init_map(){
//    var url = "map/{z}/{x}/{y}.jpg";
    var url = "http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}";
    if(g_map == null){
        g_map = L.map('googleMap',{zoomAnimation:true});
        g_map.setView(g_center,g_zoom);
        g_map.on("moveend",function(event){
            var b = g_map.getBounds();
            var zoom = g_map.getZoom();
//            window.chen.get_bounds(b.getSouth(),b.getNorth(),b.getWest(),b.getEast(),zoom);
        })
    }
    if(g_map.hasLayer(g_street))g_map.removeLayer(g_street);
    g_street = L.tileLayer(url,{ minZoom: g_minZoom,maxZoom: g_maxZoom,subdomains:['mt0','mt1','mt2','mt3']});
    g_street.addTo(g_map);
	if(g_group == null){
		g_group = L.markerClusterGroup();
		g_map.addLayer(g_group);
	}
}

/*

$(document).ready(function(){
    init_map();
//	json_parse("Z1W4.json");
//	geo_json("geojson_laneel_center_lines.json");
//	geo_json("geojson_laneel_navigable_boundaries.json");
//	geo_json("geojson_laneel_outlines.json");
   add_marker(28.1195181,112.8520119);
   add_marker(28.1114320,112.8657822);
   add_marker(28.1128113,112.8668207);
//    add_car(28.1195181,112.8520119,1);
})
*/



