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
var frontCam, perspCam = false, followCam = false, cam_change;


var clock;

var spheres = [], index = 0;
var objects = [];

var windowSize = 7.5;
var R = 75, angle2, angle1, direction, prev_pos;
var shipSphere, objectSphere, toRemove = -1;

var geometry, material, mesh, object, spaceship, planet;

var moveLeft = false, moveRight = false, moveUp = false, moveDown = false, move = false;
var move_speed = 1.5;

function createScene() {
    'use strict';

    clock = new THREE.Clock(true);
    direction = new THREE.Vector3();
    prev_pos = new THREE.Vector3();

    scene = new THREE.Scene();
    object = new THREE.Object3D();
    
    createSpaceship();
    createPlanet();

    createCube(3, 0xaacc21);
    createCube(3, 0xaacc21);
    createCube(3, 0xaacc21);
    createCube(3, 0xaacc21);
    createPolyhedron(3.5, 1, 1, 0xa11cde);
    createPolyhedron(1, 3.5, 1, 0xa11cde);
    createDiamond(1.5,  Math.PI/2, Math.PI/4, 0xffffff);
    createDiamond(1.5,  Math.PI/4, Math.PI/2, 0x2225ff);
    createBall(1.8, 0, 0xffb481);
    createBall(1.75, 5, 0xffb481);
    createBall(1.75, 5,  0xc5e18);
    createCylinders(0.5, 3.5, 0x95b4f1);
    createCylinders(0.5, 3.5, 0xff6600);
    createTube(0, 0x11f09c);
    createTube(0, 0x11f09c);
    createPyramide(1.5, 3.5, 0x823456);
    createPyramide(1.5, 3.5, 0x923456);
    createCone(1.5, 3.5, 0x8ffffa);
    createCone(1.5, 3.5, 0x8ffffa);
    createTorus(1.7, 0.5, 0xffffff);
    createTetrahedron(1.8, 0xffffff);

}

function createSpaceship(){
    spaceship = new THREE.Object3D();

    material = new THREE.MeshBasicMaterial({ color: 0xd48af0, side: THREE.DoubleSide, wireframe: false });

    geometry = new THREE.CylinderGeometry( 1.5, 1.5, 5, 16);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(0, 2.5, 0);

    spaceship.add(mesh);

    material = new THREE.MeshBasicMaterial({ color: 0x02b4c8, side: THREE.DoubleSide, wireframe: false });

    geometry = new THREE.CylinderGeometry( 1.5, 1.5, 2, 16);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(0, 6, 0);

    spaceship.add(mesh);
    
    material = new THREE.MeshBasicMaterial({ color: 0x1f973d, side: THREE.DoubleSide, wireframe: false });
    
    geometry = new THREE.CapsuleGeometry( 0.5, 0.5, 4, 8 );
    
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(1.4, 0, 1.4);
    spaceship.add(mesh);

    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(-1.4, 0, 1.4);
    spaceship.add(mesh);

    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(1.4, 0, -1.4);
    spaceship.add(mesh);

    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(-1.4, 0, -1.4);
    spaceship.add(mesh);

    angle1 = Math.random() * 2 * Math.PI;
    angle2 = Math.random() * 2 * Math.PI;
    spaceship.position.set(1.2*R * Math.sin(angle2) * Math.cos(angle1), 1.2*R * Math.cos(angle2), 1.2*R * Math.sin(angle2) * Math.sin(angle1));

    scene.add(spaceship);
    shipSphere = new THREE.Sphere(spaceship.position, 3.75);

}

function createPlanet(){
    'use strict';
    planet = new THREE.Object3D();
    
    material = new THREE.MeshBasicMaterial({ color: 0xef8800, side: THREE.DoubleSide, wireframe: false }); 

    geometry = new THREE.SphereGeometry(R, 64, 64, 0, Math.PI * 2, 0);
    mesh = new THREE.Mesh(geometry, material);
    
    planet.add(mesh);
    planet.position.set(0, 0, 0);
    
    scene.add(planet);
}

function createCylinders(radius, height, color) {
    object = new THREE.Object3D();

    material1 = new THREE.MeshBasicMaterial( { color: color, side: THREE.DoubleSide , wireframe: false} );
    
    geometry = new THREE.CylinderGeometry( radius, radius, height, 16 );
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(0, height/2+2*radius, 0);
    object.add(mesh);


    material2 = new THREE.MeshBasicMaterial( { color: color, side: THREE.DoubleSide , wireframe: false} );

    geometry = new THREE.CylinderGeometry(radius, radius, height, 16);
    mesh = new THREE.Mesh( geometry, material );
    mesh.rotation.z = Math.PI / 2;
    mesh.position.set(-height/2 + radius, radius, 0);
    object.add(mesh);
    
    var pos1 = Math.random() * 2 * Math.PI;
    var pos2 = Math.random() * 2 * Math.PI;
    object.position.set(1.2*R * Math.sin(pos2) * Math.cos(pos1), 1.2*R * Math.sin(pos2) * Math.sin(pos1), 1.2*R * Math.cos(pos2));
    scene.add(object);

    objectSphere = new THREE.Sphere(object.position, height/2);

    objects[index] = object;
    spheres[index] = objectSphere;
    index+=1;
}

function createPyramide(radius, height, color) {
    'use strict';
    object = new THREE.Object3D();
    
    material = new THREE.MeshBasicMaterial({ color: color, side: THREE.DoubleSide, wireframe: false });

    geometry = new THREE.ConeGeometry(radius, height, 4);
    mesh = new THREE.Mesh(geometry, material);
    mesh.rotation.y = Math.PI / 4;
    
    object.add(mesh);
    var pos1 = Math.random() * 2 * Math.PI;
    var pos2 = Math.random() * 2 * Math.PI;
    object.position.set(1.2*R * Math.sin(pos2) * Math.cos(pos1), 1.2*R * Math.sin(pos2) * Math.sin(pos1), 1.2*R * Math.cos(pos2));
    scene.add(object);

    objectSphere = new THREE.Sphere(object.position, height/2);

    objects[index] = object;
    spheres[index] = objectSphere;
    index+=1;

}

function createCone(radius, height, color) {
    'use strict';
    object = new THREE.Object3D();
    
    material = new THREE.MeshBasicMaterial({ color: color, side: THREE.DoubleSide, wireframe: false });

    geometry = new THREE.ConeGeometry(radius, height, 24);
    mesh = new THREE.Mesh(geometry, material);
    mesh.rotation.y = Math.PI / 4;
    
    object.add(mesh);
    var pos1 = Math.random() * 2 * Math.PI;
    var pos2 = Math.random() * 2 * Math.PI;
    object.position.set(1.2*R * Math.sin(pos2) * Math.cos(pos1), 1.2*R * Math.sin(pos2) * Math.sin(pos1), 1.2*R * Math.cos(pos2));
    scene.add(object);

    objectSphere = new THREE.Sphere(object.position, height/2);

    objects[index] = object;
    spheres[index] = objectSphere;
    index+=1;
}

function createCube(size, color) {
    createPolyhedron(size, size, size, color);
}

function createTube(rotation, color) {

    object = new THREE.Object3D();

    path = new CustomSinCurve(1);
    geometry = new THREE.TubeGeometry(path, 15, 0.2, 5, false);
    material = new THREE.MeshBasicMaterial( { color: color, side: THREE.DoubleSide , wireframe: false } );

    mesh = new THREE.Mesh( geometry, material );
    mesh.rotation.z = rotation;
    mesh.rotation.x = Math.PI;

    object.add(mesh);

    var pos1 = Math.random() * 2 * Math.PI;
    var pos2 = Math.random() * 2 * Math.PI;
    object.position.set(1.2*R * Math.sin(pos2) * Math.cos(pos1), 1.2*R * Math.sin(pos2) * Math.sin(pos1), 1.2*R * Math.cos(pos2));
    scene.add( object );

    objectSphere = new THREE.Sphere(object.position, Math.PI);

    objects[index] = object;
    spheres[index] = objectSphere;
    index+=1;
}

function createDiamond(radius, xRotation, zRotation, color) {
    object = new THREE.Object3D();

    geometry = new THREE.OctahedronGeometry(radius);
    material = new THREE.MeshBasicMaterial( { color: color, side: THREE.DoubleSide , wireframe: false } );

    mesh = new THREE.Mesh( geometry, material );
    mesh.rotation.x = xRotation;
    mesh.rotation.z = zRotation;
    
    object.add(mesh)

    var pos1 = Math.random() * 2 * Math.PI;
    var pos2 = Math.random() * 2 * Math.PI;
    object.position.set(1.2*R * Math.sin(pos2) * Math.cos(pos1), 1.2*R * Math.sin(pos2) * Math.sin(pos1), 1.2*R * Math.cos(pos2));

    scene.add( object );

    objectSphere = new THREE.Sphere(object.position, radius);

    objects[index] = object;
    spheres[index] = objectSphere;
    index+=1;
}

function createPolyhedron(xSize, ySize, zSize, color) {
    object = new THREE.Object3D();

    geometry = new THREE.BoxGeometry( xSize, ySize, zSize );
    material = new THREE.MeshBasicMaterial( { color: color, side: THREE.DoubleSide , wireframe: false} );

    mesh = new THREE.Mesh( geometry, material );
    object.add(mesh);

    var pos1 = Math.random() * 2 * Math.PI;
    var pos2 = Math.random() * 2 * Math.PI;
    object.position.set(1.2*R * Math.sin(pos2) * Math.cos(pos1), 1.2*R * Math.sin(pos2) * Math.sin(pos1), 1.2*R * Math.cos(pos2));

    scene.add(object);
    max_parameter = Math.max(xSize, ySize, zSize);

    objectSphere = new THREE.Sphere(object.position, max_parameter/2);

    objects[index] = object;
    spheres[index] = objectSphere;
    index+=1;
}

function createTorus(out_radius, in_radius, color) {

    object = new THREE.Object3D();

    geometry = new THREE.TorusGeometry( out_radius, in_radius, 16, 32 );
    material = new THREE.MeshBasicMaterial( { color: color, side: THREE.DoubleSide , wireframe: false} );

    mesh = new THREE.Mesh( geometry, material );
    mesh.rotation.y = Math.PI ;
    mesh.rotation.x = Math.PI ;

    object.add(mesh);
    
    var pos1 = Math.random() * 2 * Math.PI;
    var pos2 = Math.random() * 2 * Math.PI;
    object.position.set(1.2*R * Math.sin(pos2) * Math.cos(pos1), 1.2*R * Math.sin(pos2) * Math.sin(pos1), 1.2*R * Math.cos(pos2));

    scene.add( object );

    objectSphere = new THREE.Sphere(object.position, out_radius);

    objects[index] = object;
    spheres[index] = objectSphere;
    index+=1;
}

function createBall(radius, thetaStart, color) {
    'use strict';
    object = new THREE.Object3D();
    
    material = new THREE.MeshBasicMaterial({ color: color, side: THREE.DoubleSide, wireframe: false }); 

    geometry = new THREE.SphereGeometry(radius, 10, 10, 0, Math.PI * 2, thetaStart);
    mesh = new THREE.Mesh(geometry, material);
    
    object.add(mesh);
    var pos1 = Math.random() * 2 * Math.PI;
    var pos2 = Math.random() * 2 * Math.PI;
    object.position.set(1.2*R * Math.sin(pos2) * Math.cos(pos1), 1.2*R * Math.sin(pos2) * Math.sin(pos1), 1.2*R * Math.cos(pos2));
    
    scene.add(object);

    objectSphere = new THREE.Sphere(object.position, radius);

    objects[index] = object;
    spheres[index] = objectSphere;
    index+=1;

}

function createTetrahedron(radius, color){
    'use strict';
    object = new THREE.Object3D();
    
    material = new THREE.MeshBasicMaterial({ color: color, side: THREE.DoubleSide, wireframe: false }); 

    geometry = new THREE.TetrahedronGeometry(radius);
    mesh = new THREE.Mesh(geometry, material);
    mesh.rotation.x = Math.PI/4;
    object.add(mesh);
    var pos1 = Math.random() * 2 * Math.PI;
    var pos2 = Math.random() * 2 * Math.PI;
    object.position.set(1.2*R * Math.sin(pos2) * Math.cos(pos1), 1.2*R * Math.sin(pos2) * Math.sin(pos1), 1.2*R * Math.cos(pos2));
    
    scene.add(object);
    
   objectSphere = new THREE.Sphere(object.position, radius);

    objects[index] = object;
    spheres[index] = objectSphere;
    index+=1;
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
    else if (perspCam)
    {
        camera = new THREE.PerspectiveCamera(70,
        window.innerWidth / window.innerHeight,
        1,
        1000);
        camera.position.x = 100;
        camera.position.y = 100;
        camera.position.z = 100;
        camera.lookAt(scene.position);
    }
    else if (followCam)
    {
        camera = new THREE.PerspectiveCamera(70,
            window.innerWidth / window.innerHeight,
            1,
            1000);
        if (direction.x != 0 || direction.y != 0 || direction.z != 0){
            camera.position.x = spaceship.position.x * 1.1 - 15 * direction.x;
            camera.position.y = spaceship.position.y * 1.1 - 15 * direction.y;
            camera.position.z = spaceship.position.z * 1.1 - 15 * direction.z;
    
            camera.lookAt(spaceship.position.x + direction.x*3, spaceship.position.y + direction.y*3, spaceship.position.z + direction.z*3);
        }
        else{
            camera.position.x = spaceship.position.x;
            camera.position.y = spaceship.position.y - 15;
            camera.position.z = spaceship.position.z + 10;
            camera.lookAt(spaceship.position);
        }
        
    }
    cam_change = false;
}

function onKeyDown(e) {
    'use strict';
    
    if (e.keyCode == 49 || e.keyCode == 97){ //1
        frontCam = true;
        perspCam = false;
        followCam = false;
        cam_change = true;
    }
    if (e.keyCode == 98 || e.keyCode == 50){ //2
        frontCam = false;
        perspCam = true;
        followCam = false;
        cam_change = true;
    }
    if (e.keyCode == 99 || e.keyCode == 51){ //3
        frontCam = false;
        perspCam = false;
        followCam = true;
        cam_change = true;
    }
    if (e.keyCode == 100 || e.keyCode == 52){ //4
        wire_change = true;
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
    clock.start();
}

function onKeyUp(e) {
    'use strict';
    
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

    if (moveLeft == false && moveRight == false && moveUp == false && moveDown == false)
        move = false;

}

function onResize() {
    'use strict';

    renderer.setSize(window.innerWidth, window.innerHeight);
    
    if (frontCam) {
        if (window.innerHeight > 0 && window.innerWidth > 0) {
            camera.left = window.innerWidth / - windowSize;
            camera.right = window.innerWidth / windowSize;
            camera.top = window.innerHeight / windowSize;
            camera.bottom = window.innerHeight / -windowSize;
            camera.aspect = window.innerWidth / window.innerHeight;
        }
    }

    else if (perspCam || followCam){
        if (window.innerHeight > 0 && window.innerWidth > 0) {
            camera.aspect = window.innerWidth / window.innerHeight;
            camera.updateProjectionMatrix();
        }
    }
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
    window.addEventListener("resize", onResize);
}

function removeObject(){
    scene.remove(objects[toRemove]);
    toRemove = -1;
}

function checkCollisions() {
    for(let i in spheres)
        if (distanceToObject(objects[i]) < R)
            if(shipSphere.radius + spheres[i].radius >= distanceToObject(objects[i]))
                toRemove = i;     
}

function distanceToObject(object){
    return Math.sqrt( (spaceship.position.x - object.position.x)**2 + (spaceship.position.y - object.position.y)**2 + (spaceship.position.z - object.position.z)**2 );
}

function moveSpaceship() {

    seconds = clock.getDelta();

    if (moveLeft){
        angle1 -= move_speed * seconds;
    }

    if (moveRight){
        angle1 += move_speed * seconds;
    }

    if (moveUp){
        angle2 -= move_speed * seconds;
    }

    if (moveDown){
        angle2 += move_speed * seconds;
    }

    prev_pos.x = spaceship.position.x;
    prev_pos.y = spaceship.position.y;
    prev_pos.z = spaceship.position.z;

    spaceship.position.x = 1.2*R * Math.sin(angle2) * Math.cos(angle1);
    spaceship.position.y = 1.2*R * Math.cos(angle2);
    spaceship.position.z = 1.2*R * Math.sin(angle2) * Math.sin(angle1);

    direction.x = spaceship.position.x - prev_pos.x;
    direction.y = spaceship.position.y - prev_pos.y;
    direction.z = spaceship.position.z - prev_pos.z;

    direction.normalize();
}

function moveFollowCam() {

    if (direction.x != 0 || direction.y != 0 || direction.z != 0){
        camera.position.x = spaceship.position.x * 1.1 - 15 * direction.x;
        camera.position.y = spaceship.position.y * 1.1 - 15 * direction.y;
        camera.position.z = spaceship.position.z * 1.1 - 15 * direction.z;

        camera.lookAt(spaceship.position.x + direction.x*3, spaceship.position.y + direction.y*3, spaceship.position.z + direction.z*3);
    }
}

function update() {

    if (cam_change)
        changeCamera();
    
    if (toRemove != -1)
        removeObject();

    if (move){
        moveSpaceship();
        checkCollisions();

        if (followCam)
            moveFollowCam();
    }
        
    clock.start();
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

