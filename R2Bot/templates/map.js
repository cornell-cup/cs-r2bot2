function zeros(dimensions) {
    var array = [];

    for (var i = 0; i < dimensions[0]; ++i) {
        array.push(dimensions.length == 1 ? 0 : zeros(dimensions.slice(1)));
    }

    array[15][15] = 1;

    return array;
}

var canvas1 = document.getElementById('main');
var mctx = canvas1.getContext('2d');
var dict = {};
function drawMap(dist) {
    var map = zeros([30, 30]);
    mctx.clearRect(0, 0, 301, 301);
    //splitting up array of sensor data and creating dictionary of sensor:sensorData
    var n = dist.split("|");
    //console.log(n);

    for (var i = 0; i < n.length; i++) {
        var split = n[i].split(',');
        console.log(n[i]);
        if (parseFloat(split[1]) <= 20.0 && parseFloat(split[1]) > 4) {
            dict[split[0]] = parseFloat(split[1]) / 2;
        }
    }
    console.log(dict);
    //storing values in sensor and distance 
    for (var key in dict) {
        var sensor = key;
        var distance = dict[key];
        angle = parseFloat(getAngle(sensor)) * Math.PI / 180;
        findEdge(distance, angle, sensor);
    }

    for (var i = 0; i < map.length; i++) {
        for (var j = 0; j < map[i].length; j++) {
            if (map[i][j] === 1) {
                mctx.fillStyle = 'Red';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 2) {
                mctx.fillStyle = 'Yellow';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 3) {
                mctx.fillStyle = 'Cyan';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 4) {
                mctx.fillStyle = 'Green';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 5) {
                mctx.fillStyle = 'DarkBlue';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 6) {
                mctx.fillStyle = 'Pink';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 7) {
                mctx.fillStyle = 'Aquamarine';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 8) {
                mctx.fillStyle = 'Beige';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 9) {
                mctx.fillStyle = 'Coral';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 10) {
                mctx.fillStyle = 'Crimson';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 11) {
                mctx.fillStyle = 'MediumOrchid';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 12) {
                mctx.fillStyle = 'PaleVioletRed';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 13) {
                mctx.fillStyle = 'Tomato';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 14) {
                mctx.fillStyle = 'SkyBlue';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }

        }
    }

    function findEdge(distance, angle, sensor) {
        //R2 position = map[15][15]

        var centerAngle = angle;
        for (var i = 0; i < 4; i++) { //going counterclockwise
            var xComponent = Math.floor(distance * Math.cos(centerAngle));
            var yComponent = Math.floor(distance * Math.sin(centerAngle));
            map[15 - yComponent][15 + xComponent] = getColor(sensor);
            centerAngle += 5 * Math.PI / 180;
        }

        for (var i = 0; i < 4; i++) { //going clockwise
            var xComponent = Math.floor(distance * Math.cos(angle));
            var yComponent = Math.floor(distance * Math.sin(angle));
            map[15 - yComponent][15 + xComponent] = getColor(sensor);
            angle -= 5 * Math.PI / 180;
        }
        //column, row
    }
    function getColor(sensor) {
        var color = 0;
        switch (sensor) {
            case "U1SENSOR":
                color = 2;
                break;
            case "U2SENSOR":
                color = 3;
                break;
            case "U3SENSOR":
                color = 4;
                break;
            case "U4SENSOR":
                color = 5;
                break;
            case "U5SENSOR":
                color = 6;
                break;
            case "U6SENSOR":
                color = 7;
                break;
            case "U7SENSOR":
                color = 8;
                break;
            case "U8SENSOR":
                color = 9;
                break;
            case "U9SENSOR":
                color = 10;
                break;
            case "U10SENSOR":
                color = 11;
                break;
            case "U11SENSOR":
                color = 12;
                break;
            case "U12SENSOR":
                color = 13;
                break;
            case "U13SENSOR":
                color = 14;
                break;
            case "U14SENSOR":
                color = 15;
                break;
            }

        return color;
    }
    function getAngle(sensor) {
        var angle = 0;
        switch (sensor) {
            case "U1SENSOR":
                angle = 25;
                break;
            case "U2SENSOR":
                angle = 50;
                break;
            case "U3SENSOR":
                angle = 75;
                break;
            case "U4SENSOR":
                angle = 100;
                break;
            case "U5SENSOR":
                angle = 125;
                break;
            case "U6SENSOR":
                angle = 150;
                break;
            case "U7SENSOR":
                angle = 175;
                break;
            case "U8SENSOR":
                angle = 200;
                break;
            case "U9SENSOR":
                angle = 225;
                break;
            case "U10SENSOR":
                angle = 250;
                break;
            case "U11SENSOR":
                angle = 275;
                break;
            case "U12SENSOR":
                angle = 300;
                break;
            case "U13SENSOR":
                angle = 325;
                break;
            case "U14SENSOR":
                angle = 350;
                break;
        }

        return angle;
    }
}

function drawBoard() {
    for (var x = 0; x <= 301; x += 10) {
        mctx.moveTo(0.5 + x + 0, 0);
        mctx.lineTo(0.5 + x + 0, 300 + 0);
    }

    for (var x = 0; x <= 301; x += 10) {
        mctx.moveTo(0, 0.5 + x + 0);
        mctx.lineTo(300 + 0, 0.5 + x + 0);
    }

    mctx.strokeStyle = "black";
    mctx.stroke();

}