# $Id: system.pro,v 1.1 2002/03/13 14:38:30 pavlovskii Exp $

key KernelDebug
    Enabled=true
end

key Devices
    # This is a list of device=driver pairs which get loaded at startup
    fdc0=fdc
    keyboard=keyboard
    tty0=tty
    tty1=tty
    tty2=tty
    tty3=tty
    tty4=tty
    tty5=tty
    tty6=tty
    pci=pci
    cmos=cmos
    ps2mouse=ps2mouse   
end

key PCI
    key Vendor8086Device7190
	Description=Intel 82443BX/ZX 440BX/ZX CPU to PCI Bridge (AGP Implemented)
    end

    key Vendor8086Device7191
	Description=Intel 82443BX/ZX 440BX/ZX PCI to AGP Bridge
    end

    key Vendor8086Device7110
	Description=Intel 82371AB/EB/MB PIIX4 ISA Bridge
    end

    key Vendor8086Device7111
	Description=Intel 82371AB/EB/MB PIIX4 EIDE Controller
	Driver=ata
    end

    key Vendor8086Device7112
	Description=Intel 82371AB/EB/MB PIIX4 USB Controller
    end
    
    key Vendor8086Device7113
	Description=Intel 82371AB/EB/MB PIIX4 Power Management Controller
    end

    key Vendor5333Device8811
	Description=S3 86C732 Trio32, 86C764 Trio64, 86C765 Trio64V+ Rev 01
	Driver=video
	Device=video
    end
end
