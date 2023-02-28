# Ecs Basics

This is a simple example that demonstrates the basics of the ECS (Entity Component System) in Quark.

The example showcases how to create, read, update, and delete a component called **CustomTimer** from entities using the functions provided in the engine.

## Controls
No controls.

## Build
A build is available in [bin/ecs_basics/](bin/ecs_basics/).

## Output
After a couple of seconds the output should look something like this:
```
[MESSAGE] Added timers!
CustomTimer finished! (Entity ID: 5)
CustomTimer finished! (Entity ID: 9)
CustomTimer finished! (Entity ID: 7)
CustomTimer finished! (Entity ID: 6)
CustomTimer finished! (Entity ID: 4)
CustomTimer finished! (Entity ID: 1)
CustomTimer finished! (Entity ID: 8)
CustomTimer finished! (Entity ID: 0)
CustomTimer finished! (Entity ID: 3)
CustomTimer finished! (Entity ID: 2)
[MESSAGE] Timers finished!
```

The program prints out when the "CustomTimer"'s finish, then once all timers have finished it creates 10 more.


