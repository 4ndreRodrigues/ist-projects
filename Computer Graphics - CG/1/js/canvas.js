/*global THREE, requestAnimationFrame, console*/

class CustomSinCurve extends THREE.Curve {

	constructor( scale = 1 ) {

		super();

		this.scale = scale;

	}

	getPoint( t, optionalTarget = new THREE.Vector3() ) {

		const tx = t * 3 - 1.5;
		const ty = Math.sin( 2 * Math.PI * t );
		const tz = 0;

		return optionalTarget.set( tx, ty, tz ).multiplyScalar( this.scale );

	}

}

var camera, scene, renderer;
var frontCam, upCam = false, sideCam = false, cam_change;
var wire_change;

var time;
var materials = [], index = 0;

var windowSize = 8;

var geometry, material, mesh, object;

var obj0, obj1, obj2;

var rotate0Down = false, rotate0Up = false, rotate1Pos = false, rotate1Neg = false, rotate2Pos = false, rotate2Neg = false, rotate = false;
var rotate_speed = 0.002;

var moveLeft = false, moveRight = false, moveUp = false, moveDown = false, moveClose = false, moveFar = false, move = false;
var move_speed = 0.02;

function createScene() {
    'use strict';

    scene = new THREE.Scene();
    object = new THREE.Object3D();

    obj0 = new THREE.Object3D();
    obj1 = new THREE.Object3D();
    obj2 = new THREE.Object3D();
    
    createArticulate();
    createPyramide(15, 40, 0, 0xf9b300, 0, -40, 30); //lower

    createTorus(10, 2.5, 0xd60206, 50, -50, 10); //closer
    createTorus(10, 2.5, 0xffffff, 50, -50, -30); //further
    createCube(0, 7, 0x5f72bb, 50, -40, -10); //close to torus
    createCylinder(1.5, 40, -90, 0x95b4f1, -40, -13, 40); //lower
    createCylinder(1.5, 40, 90, 0xff6600, -40, 23, 20); //upper
    createBall(6.5, 0, 0xffb481, -43, 5, 30);
    createBall(6.5, 5, 0xc5e186, 43, 35, 30); //half
    createTube(48.7, 0xb77000,-50, -40, -10);
    createCube(90, 3, 0xff9046, -75, -33, -10);
    createPolyhedron(0, 25, 3, 10, 0xffff00, -50, -55, -10); //horizontal
    createPolyhedron(Math.PI/2, 25, 3, 10, 0x0099ff, -36.5, -41, -10); //vertical    
    createPolyhedron(-Math.PI/3, 15, 3, 10, 0x00ffff, -67, -47.5, -10);
    createPolyhedron(Math.PI/2, 15, 35, 10, 0x876543, 50, 0, -50); // close to diamond
    createDiamond(10, Math.PI/2, Math.PI/4, 0xffffff, -80, 33, 50); //bigger
    createDiamond(6, Math.PI/2, Math.PI/4, 0x2225ff, 50, 8, -10); //smaller
    
}

function createArticulate(){
    obj0 = new THREE.Object3D();
    obj1 = new THREE.Object3D();
    obj2 = new THREE.Object3D();

    material = new THREE.MeshBasicMaterial({ color: 0xd48af0, side: THREE.DoubleSide, wireframe: true });
    materials[index] = material;
    index+=1;

    geometry = new THREE.ConeGeometry(6, 17, 24);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(0, 8.5, 0);

    obj2.add(mesh);
    obj2.position.set(0, 23, 0);

    material = new THREE.MeshBasicMaterial({ color: 0x02b4c8, side: THREE.DoubleSide, wireframe: true });
    materials[index] = material;
    index+=1;

    geometry = new THREE.ConeGeometry(9, 23, 4);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(0, 11.5, 0);

    obj1.add(obj2);
    obj1.add(mesh);
    obj1.rotation.y = Math.PI / 4;
    obj1.position.set(0, 1.55, 0);

    material = new THREE.MeshBasicMaterial({ color: 0x1f973d, side: THREE.DoubleSide, wireframe: true });
    materials[index] = material;
    index+=1;

    geometry = new THREE.CubeGeometry(40, 0.1, 40);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(0, 0.05, 0);

    obj0.add(mesh);
    obj0.add(obj1);
    obj0.position.set(0, -13, 0);

    scene.add(obj0);

}

function createPyramide(radius, height, articulate, color, x, y, z) {
    'use strict';
    object = new THREE.Object3D();
    
    material = new THREE.MeshBasicMaterial({ color: color, side: THREE.DoubleSide, wireframe: true });
    materials[index] = material;
    index+=1;

    geometry = new THREE.ConeGeometry(radius, height, 4);
    mesh = new THREE.Mesh(geometry, material);
    mesh.rotation.y = Math.PI / 4;
    mesh.position.set(x, y, z);
    
    switch (articulate)
    {
        case 1:    
            obj1.add(mesh);
            break;
        case 2:
            obj2.add(mesh);
            break;
        case 0:
            object.add(mesh);
            scene.add(object);
            break;
    }
}

function createCube(rotation, size, color, x, y, z) {
    createPolyhedron(rotation, size, size, size, color, x, y, z);
}

function createTube(rotation, color, x, y, z) {

    object = new THREE.Object3D();

    path = new CustomSinCurve(3);
    geometry = new THREE.TubeGeometry(path, 15, 1, 5, false);
    material = new THREE.MeshBasicMaterial( { color: color, side: THREE.DoubleSide , wireframe: true } );
    materials[index] = material;
    index+=1;

    mesh = new THREE.Mesh( geometry, material );
    mesh.rotation.z = rotation;
    mesh.rotation.x = Math.PI;

    object.add(mesh);
    object.position.set(x, y, z);

    scene.add( object );
}

function createDiamond(radius, xRotation, zRotation, color, x, y, z) {
    object = new THREE.Object3D();

    geometry = new THREE.OctahedronGeometry(radius);
    material = new THREE.MeshBasicMaterial( { color: color, side: THREE.DoubleSide , wireframe: true } );
    materials[index] = material;
    index+=1;

    mesh = new THREE.Mesh( geometry, material );
    mesh.rotation.x = xRotation;
    mesh.rotation.z = zRotation;
    
    object.add(mesh)
    object.position.set(x, y, z);

    scene.add( object );
}

function createCylinder(radius, height, rotation, color, x, y, z) {
    object = new THREE.Object3D();

    geometry = new THREE.CylinderGeometry( radius, radius, height, 16 );
    material = new THREE.MeshBasicMaterial( { color: color, side: THREE.DoubleSide , wireframe: true} );
    materials[index] = material;
    index+=1;

    mesh = new THREE.Mesh( geometry, material );
    mesh.rotation.z = rotation;

    object.add(mesh)
    object.position.set(x, y, z);

    scene.add( object );
}

function createPolyhedron(rotation, xSize, ySize, zSize, color, x, y, z) {
    object = new THREE.Object3D();

    geometry = new THREE.CubeGeometry( xSize, ySize, zSize );
    material = new THREE.MeshBasicMaterial( { color: color, side: THREE.DoubleSide , wireframe: true} );
    materials[index] = material;
    index+=1;

    mesh = new THREE.Mesh( geometry, material );
    mesh.rotation.z = rotation;

    object.add(mesh);
    object.position.set(x, y, z);

    scene.add(object);
}

function createTorus(in_radius, out_radius, color, x, y, z ) {

    object = new THREE.Object3D();

    geometry = new THREE.TorusGeometry( in_radius, out_radius, 16, 32 );
    material = new THREE.MeshBasicMaterial( { color: color, side: THREE.DoubleSide , wireframe: true} );
    materials[index] = material;
    index+=1;

    mesh = new THREE.Mesh( geometry, material );
    mesh.rotation.x = Math.PI / 2;

    object.add(mesh);
    object.position.set(x, y, z);

    scene.add( object );
}

function createBall(radius, thetaStart, color, x, y, z) {
    'use strict';
    object = new THREE.Object3D();
    
    material = new THREE.MeshBasicMaterial({ color: color, side: THREE.DoubleSide, wireframe: true }); 
    materials[index] = material;
    index+=1;

    geometry = new THREE.SphereGeometry(radius, 10, 10, 0, Math.PI * 2, thetaStart);
    mesh = new THREE.Mesh(geometry, material);
    
    object.add(mesh);
    object.position.set(x, y, z);
    
    scene.add(object);

}

function createCamera() {
    'use strict';

    camera = new THREE.OrthographicCamera(window.innerWidth / - windowSize,
        window.innerWidth / windowSize,
        window.innerHeight / windowSize,
        window.innerHeight / -windowSize,
        1,
        200);
    camera.position.x = 0;
    camera.position.y = 0;
    camera.position.z = 100;
    camera.lookAt(scene.position);
}

function changeCamera() {
    'use strict';

    if (frontCam)
    {
        camera.position.x = 0;
        camera.position.y = 0;
        camera.position.z = 100;
    }
    else if (upCam)
    {
        camera.position.x = 0;
        camera.position.y = 100;
        camera.position.z = 0;
    }
    else if (sideCam)
    {
        camera.position.x = 100;
        camera.position.y = 0;
        camera.position.z = 0;
    }

    camera.lookAt(scene.position);
    cam_change = false;
}

function onKeyDown(e) {
    'use strict';
    
    if (e.keyCode == 49 || e.keyCode == 97){ //1
        frontCam = true;
        upCam = false;
        sideCam = false;
        cam_change = true;
    }
    if (e.keyCode == 98 || e.keyCode == 50){ //2
        frontCam = false;
        upCam = true;
        sideCam = false;
        cam_change = true;
    }
    if (e.keyCode == 99 || e.keyCode == 51){ //3
        frontCam = false;
        upCam = false;
        sideCam = true;
        cam_change = true;
    }
    if (e.keyCode == 100 || e.keyCode == 52){ //4
        wire_change = true;
    }
    if (e.keyCode == 81){ //Q
        rotate0Down = true;  
        rotate = true;
    }
    if (e.keyCode == 87){ //W
        rotate0Up = true;  
        rotate = true;
    }
    if (e.keyCode == 65){ //A
        rotate1Pos = true;  
        rotate = true;
    }
    if (e.keyCode == 83){ //S
        rotate1Neg = true; 
        rotate = true;
    }
    if (e.keyCode == 90){ //Z
        rotate2Pos = true; 
        rotate = true;
    }
    if (e.keyCode == 88){ //X
        rotate2Neg = true;
        rotate = true;
    }
    if (e.keyCode == 37){ //Left Arrow
        moveLeft = true;  
        move = true;
    }
    if (e.keyCode == 38){ //Up Arrow
        moveUp = true;  
        move = true;
    }
    if (e.keyCode == 39){ //Right Arrow
        moveRight = true;  
        move = true;
    }
    if (e.keyCode == 40){ //Down Arrow
        moveDown = true;  
        move = true;
    }
    if (e.keyCode == 68){ //D
        moveFar = true;  
        move = true;
    }
    if (e.keyCode == 67){ //C
        moveClose = true;  
        move = true;
    }
    time = new Date();
}

function onKeyUp(e) {
    'use strict';
    
    if (e.keyCode == 81) { //Q
        rotate0Down = false;
    }
    if (e.keyCode == 87) { //W
        rotate0Up = false;
    }
    if (e.keyCode == 65) { //A
        rotate1Pos = false;
    }
    if (e.keyCode == 83) { //S
        rotate1Neg = false;
    }
    if (e.keyCode == 90) { //Z
        rotate2Pos = false;
    }
    if (e.keyCode == 88) { //X
        rotate2Neg = false;
    }
    if (e.keyCode == 37) { //Left Arrow
        moveLeft = false;
    }
    if (e.keyCode == 38) { //Up Arrow
        moveUp = false;
    }
    if (e.keyCode == 39) { //Right Arrow
        moveRight = false;
    }
    if (e.keyCode == 40) { //Down Arrow
        moveDown = false;
    }
    if (e.keyCode == 68) { //D
        moveFar = false;
    }
    if (e.keyCode == 67) { //C
        moveClose = false;
    }

    if (moveLeft == false && moveRight == false && moveUp == false && moveDown == false && moveFar == false && moveClose == false)
        move = false;
    
    if (rotate0Down == false && rotate0Up == false && rotate1Pos == false && rotate1Neg == false && rotate2Pos == false && rotate2Neg == false)
        rotate = false;
}

function render() {
    'use strict';
    renderer.render(scene, camera);
}

function init() {
    'use strict';
    renderer = new THREE.WebGLRenderer({
        antialias: true
    });
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);
   
    createScene();

    frontCam = true;
    createCamera();
    
    window.addEventListener("keydown", onKeyDown);
    window.addEventListener("keyup", onKeyUp);
}

function changeWireFrame() {
    /*scene.traverse(function (node) {
        if (node instanceof THREE.Mesh) {
            node.material.wireframe = !node.material.wireframe;
        }
    });*/
    for (let i in materials)
        materials[i].wireframe = !material.wireframe;
    wire_change = false;

}

function rotateObjects() {

    if (rotate0Down)
        obj0.rotation.x += rotate_speed * (new Date() - time);

    if (rotate0Up)
        obj0.rotation.x -= rotate_speed * (new Date() - time);

    if (rotate1Pos)
        obj1.rotation.y += rotate_speed * (new Date() - time);

    if (rotate1Neg)
        obj1.rotation.y -= rotate_speed * (new Date() - time);

    if (rotate2Pos)
        obj2.rotation.z += rotate_speed * (new Date() - time);

    if (rotate2Neg)
        obj2.rotation.z -= rotate_speed * (new Date() - time);
    
}

function moveObjects() {
    if (moveLeft)
        obj0.position.x -= move_speed * (new Date() - time);

    if (moveRight)
        obj0.position.x += move_speed * (new Date() - time);

    if (moveUp)
        obj0.position.y += move_speed * (new Date() - time);

    if (moveDown)
        obj0.position.y -= move_speed * (new Date() - time);

    if (moveFar)
        obj0.position.z -= move_speed * (new Date() - time);

    if (moveClose)
        obj0.position.z += move_speed * (new Date() - time);

}

function update() {

    if (cam_change)
        changeCamera();

    if (wire_change)
        changeWireFrame();

    if (rotate)
        rotateObjects();
        
    if (move)
        moveObjects();

    time = new Date();
    
}

function display() {
    render();
}

function animate() {
    'use strict';

    update();
    
    display();
    
    requestAnimationFrame(animate);
}

